# minictl — Lightweight Linux Container Runtime from Scratch

Minimal Linux container runtime written in **C** against raw Linux kernel syscalls (`clone`, `pivot_root`, `mount`) and **cgroups v2**, isolating untrusted workloads without any background daemon dependencies.

## Kernel Primitives Used

- **Namespaces (`src/container.c`):** Isolates process trees (`CLONE_NEWPID`), mount tables (`CLONE_NEWNS` with `MS_PRIVATE` propagation), hostnames (`CLONE_NEWUTS`), IPC (`CLONE_NEWIPC`), and network stacks (`CLONE_NEWNET`).
- **Filesystem Jail (`pivot_root`):** Swaps the root mount to an unpacked Alpine Linux rootfs and detaches the host root (`MNT_DETACH`) to prevent `chroot` directory traversal escapes.
- **Resource Quotas (`src/cgroups.c`):** Creates dedicated **cgroups v2** slices enforcing hard caps on RAM (`memory.max`) and process count (`pids.max = 64`) to neutralize fork bombs.

## Build & Run (Linux Kernel 5.x/6.x with cgroups v2)

```bash
make
sudo ./minictl ./rootfs/alpine /bin/sh
```
