#! /bin/bash
set -ex

cd /

mount_root=/mnt/rootfs

rm -rf /output/*

cp /root/linux-source-$KERNEL_SOURCE_VERSION/vmlinux /output/vmlinux
cp /root/linux-source-$KERNEL_SOURCE_VERSION/.config /output/config

truncate -s 1G /output/image.ext4
mkfs.ext4 /output/image.ext4

mkdir -p $mount_root
mount /output/image.ext4 $mount_root
debootstrap --include openssh-server,netplan.io,nano,curl focal $mount_root http://archive.ubuntu.com/ubuntu/

mkdir -p $mount_root/root
mkdir -p $mount_root/script
# cp /root/linux*.deb $mount_root/root/
mount --bind /root $mount_root/root
mount --bind /script $mount_root/script
mount -t proc /proc $mntdir/proc
mount -t sysfs /sys $mount_root/sys
mount --bind /dev $mount_root/dev
mount --bind /dev/pts $mount_root/dev/pts

chroot $mount_root /bin/bash /script/provision.sh

umount $mount_root/dev/pts
umount $mount_root/dev
umount $mount_root/sys
# umount $mount_root/proc
umount $mount_root/script
umount $mount_root/root
umount $mount_root

cd /output
tar czvf ubuntu-focal.tar.gz image.ext4 vmlinux config
cd /
