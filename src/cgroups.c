#include "minictl.h"
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

static void write_file(const char *path, const char *val) {
    FILE *f = fopen(path, "w");
    if (f) { fprintf(f, "%s", val); fclose(f); }
}

int setup_cgroup_v2(const char *id, int pid, size_t mem_mb) {
    char dir[256], path[300], buf[64];
    snprintf(dir, sizeof(dir), "/sys/fs/cgroup/minictl_%s", id);
    mkdir(dir, 0755);
    snprintf(path, sizeof(path), "%s/memory.max", dir);
    snprintf(buf, sizeof(buf), "%zu", mem_mb * 1024 * 1024);
    write_file(path, buf);
    snprintf(path, sizeof(path), "%s/pids.max", dir);
    write_file(path, "64");
    snprintf(path, sizeof(path), "%s/cgroup.procs", dir);
    snprintf(buf, sizeof(buf), "%d", pid);
    write_file(path, buf);
    return 0;
}

void cleanup_cgroup_v2(const char *id) {
    char dir[256];
    snprintf(dir, sizeof(dir), "/sys/fs/cgroup/minictl_%s", id);
    rmdir(dir);
}
