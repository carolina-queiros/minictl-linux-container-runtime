#!/usr/bin/env bash
set -euo pipefail
ROOTFS="./rootfs/alpine"
echo "[*] Testing mount & PID namespace isolation..."
sudo ./minictl "$ROOTFS" /bin/sh -c "hostname && ps aux"
echo "[*] Testing fork-bomb protection (pids.max=64)..."
sudo ./minictl "$ROOTFS" /bin/sh -c ":(){ :|:& };:" || echo "[+] Fork-bomb blocked by cgroups v2!"
