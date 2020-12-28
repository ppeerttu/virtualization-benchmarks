#!/bin/bash

GREEN='\033[0;32m'
RED='\033[31m'
NC='\033[0m' # No Color
host_linux_version=$(uname -r)
host_arch=$(uname -m)
firecracker_dir=~/.firecracker
firecracker_version=v0.23.1
# $(basename $(curl -fsSLI -o /dev/null -w  %{url_effective} https://github.com/firecracker-microvm/firecracker/releases/latest))
kernel_file_name=ubuntu-vmlinux
rootfs_file_name=ubuntu-rootfs.ext4
kernel_file=$firecracker_dir/$kernel_file_name
rootfs_file=$firecracker_dir/$rootfs_file_name
rootfs_final_size=30G
guest_memory=3934
guest_vcpu=4

iptables_backup=$firecracker_dir/iptables.rules.old
tap_name=tap0
tap_cidr=172.16.0.1/24
host_interface=br0
guest_id_file=$firecracker_dir/.guest_id

if [ -f $guest_id_file ]; then
    guest_id=$(cat $guest_id_file)
fi

log_info() {
    printf "%(%Y-%m-%d %H:%M:%S)T - ${GREEN}INFO${NC} - $1\n"
}

log_error() {
    printf "%(%Y-%m-%d %H:%M:%S)T - ${RED}ERROR${NC} - $1\n"
}
