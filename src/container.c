#define _GNU_SOURCE
#include "minictl.h"
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>
#define STACK_SIZE (1024 * 1024)
static int pivot_root(const char *nr, const char *po) { return syscall(SYS_pivot_root, nr, po); }
static int child_fn(void *arg) {
    struct container_config *cfg = (struct container_config *)arg;
    sethostname(cfg->hostname, strlen(cfg->hostname));
    mount(NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL);
    mount(cfg->rootfs, cfg->rootfs, NULL, MS_BIND | MS_REC, NULL);
    char put_old[512];
    snprintf(put_old, sizeof(put_old), "%s/.pivot_old", cfg->rootfs);
    mkdir(put_old, 0755);
    pivot_root(cfg->rootfs, put_old);
    chdir("/");
    umount2("/.pivot_old", MNT_DETACH);
    rmdir("/.pivot_old");
    mount("proc", "/proc", "proc", MS_NOSUID | MS_NOEXEC | MS_NODEV, NULL);
    execvp(cfg->argv[0], cfg->argv);
    return 1;
}
int run_container(struct container_config *cfg) {
    char *stack = malloc(STACK_SIZE);
    if (!stack) return -1;
    int pid = clone(child_fn, stack + STACK_SIZE, CLONE_NEWPID | CLONE_NEWNS | CLONE_NEWUTS | CLONE_NEWIPC | CLONE_NEWNET | SIGCHLD, cfg);
    setup_cgroup_v2(cfg->id, pid, cfg->mem_limit_mb);
    waitpid(pid, NULL, 0);
    cleanup_cgroup_v2(cfg->id);
    free(stack);
    return 0;
}
