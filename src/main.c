#include "minictl.h"
#include <stdio.h>
int main(int argc, char **argv) {
    if (argc < 3) { fprintf(stderr, "Usage: %s <rootfs> <cmd>\n", argv[0]); return 1; }
    struct container_config cfg = {"sandbox-01", "minictl-box", argv[1], 128, &argv[2]};
    return run_container(&cfg);
}
