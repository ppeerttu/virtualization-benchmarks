#!/bin/bash

GREEN='\033[0;32m'
NC='\033[0m' # No Color
host_linux_version=$(uname -r)
host_arch=$(uname -m)
firecracker_dir=~/.firecracker
firecracker_version=$(basename $(curl -fsSLI -o /dev/null -w  %{url_effective} https://github.com/firecracker-microvm/firecracker/releases/latest))
kernel_file=$firecracker_dir/ubuntu-vmlinux
rootfs_file=$firecracker_dir/ubuntu-rootfs.ext4
rootfs_final_size=5G
iptables_backup=$firecracker_dir/iptables.rules.old
tap_name=tap0
tap_cidr=172.16.0.1/24
host_interface=br0

log_info() {
    printf "%(%Y-%m-%d %H:%M:%S)T - ${GREEN}INFO${NC} - $1\n"
}
