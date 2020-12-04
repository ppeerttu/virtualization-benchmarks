#! /bin/bash
set -ex

rm -rf /output/*

cp /root/linux-source-$KERNEL_SOURCE_VERSION/vmlinux /output/vmlinux
cp /root/linux-source-$KERNEL_SOURCE_VERSION/.config /output/config

truncate -s 1G /output/image.ext4
mkfs.ext4 /output/image.ext4

mount /output/image.ext4 /rootfs
debootstrap --include openssh-server,netplan.io,nano focal /rootfs http://archive.ubuntu.com/ubuntu/
mkdir -p /rootfs/mnt/root
mkdir -p /rootfs/mnt/script
mount --bind /root /rootfs/mnt/root
mount --bind /script /rootfs/mnt/script

sleep 10
chroot /rootfs /bin/bash /mnt/script/provision.sh

umount /rootfs/mnt/script
# Somehow this does not work, so skip
# umount /rootfs/mnt/root
umount /rootfs

cd /output
tar czvf ubuntu-focal.tar.gz image.ext4 vmlinux config
cd /
