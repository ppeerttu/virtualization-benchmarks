#!/bin/bash

host_linux_version=$(uname -r)
host_arch=$(uname -m)
firecracker_dir=~/.firecracker
firecracker_version=$(basename $(curl -fsSLI -o /dev/null -w  %{url_effective} https://github.com/firecracker-microvm/firecracker/releases/latest))
kernel_file=$firecracker_dir/ubuntu-vmlinux
rootfs_file=$firecracker_dir/ubuntu-rootfs.ext4
rootfs_final_size=5G
