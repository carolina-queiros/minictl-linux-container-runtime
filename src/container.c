#define _GNU_SOURCE
#include "minictl.h"
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/wait.h>
#include <unistd.h>
#define STACK_SIZE (1024 * 1024)
static int child_fn(void *arg) {
    struct container_config *cfg = (struct container_config *)arg;
    sethostname(cfg->hostname, strlen(cfg->hostname));
    if (chroot(cfg->rootfs) != 0) { perror("chroot"); return 1; }
    chdir("/");
    mount("proc", "/proc", "proc", 0, NULL);
    execvp(cfg->argv[0], cfg->argv);
    return 1;
}
int run_container(struct container_config *cfg) {
    char *stack = malloc(STACK_SIZE);
    if (!stack) return -1;
    int pid = clone(child_fn, stack + STACK_SIZE, CLONE_NEWPID | CLONE_NEWNS | CLONE_NEWUTS | CLONE_NEWIPC | SIGCHLD, cfg);
    waitpid(pid, NULL, 0);
    free(stack);
    return 0;
}
