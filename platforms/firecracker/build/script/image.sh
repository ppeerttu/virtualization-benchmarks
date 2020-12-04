#! /bin/bash
set -ex

mount_root=/rootfs

rm -rf /output/*

cp /root/linux-source-$KERNEL_SOURCE_VERSION/vmlinux /output/vmlinux
cp /root/linux-source-$KERNEL_SOURCE_VERSION/.config /output/config

truncate -s 1G /output/image.ext4
mkfs.ext4 /output/image.ext4

mkdir -p $mount_root
mount /output/image.ext4 $mount_root
debootstrap --include openssh-server,netplan.io,nano focal $mount_root http://archive.ubuntu.com/ubuntu/

mount --bind /root $mount_root/mnt/root
mount --bind /script $mount_root/mnt/script

chroot $mount_root /bin/bash /mnt/script/provision.sh
umount $mount_root/mnt/script
umount $mount_root/mnt/root
umount $mount_root

cd /output
tar czvf ubuntu-focal.tar.gz image.ext4 vmlinux config
cd /

I: Extracting zlib1g...
W: Failure trying to run: chroot "/rootfs" mount -t proc proc /proc
W: See /rootfs/debootstrap/debootstrap.log for details
I: Installing core packages...
