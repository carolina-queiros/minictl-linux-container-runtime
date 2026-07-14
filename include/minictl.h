#ifndef MINICTL_H
#define MINICTL_H
#include <stddef.h>
struct container_config {
    const char *id;
    const char *hostname;
    const char *rootfs;
    size_t mem_limit_mb;
    char **argv;
};
int run_container(struct container_config *cfg);
int setup_cgroup_v2(const char *id, int pid, size_t mem_mb);
void cleanup_cgroup_v2(const char *id);
#endif
