#!/bin/bash

# Download the Ubuntu base
curl -LOJ http://cdimage.ubuntu.com/ubuntu-base/releases/20.04/release/ubuntu-base-20.04.1-base-amd64.tar.gz
# tar -xzf ubuntu*.tar.gz

# Allocate rootfs image
sudo dd if=/dev/zero of=rootfs.img bs=1024 count=1M
sudo mkfs.ext4 -F -L linuxroot rootfs.img

sudo mkdir /mnt/tmpdir
sudo mount -o loop rootfs.img /mnt/tmpdir

# Unpack the downloaded Ubuntu base image
sudo tar zxvf ubuntu-*.tar.gz -C /mnt/tmpdir

# Setup the image
sudo cp /etc/resolv.conf /mnt/tmpdir/etc/
sudo mount -t proc /proc /mnt/tmpdir/proc
sudo mount -t sysfs /sys /mnt/tmpdir/sys
sudo mount -o bind /dev /mnt/tmpdir/dev
sudo mount -o bind /dev/pts /mnt/tmpdir/dev/pts

# Run inside the image
sudo chroot /mnt/tmpdir

# Install packages
apt-get update
apt-get install -y \
    language-pack-en-base \
    sudo \
    ssh \
    net-tools \
    ethtool \
    wireless-tools \
    ifupdown \
    network-manager \
    iputils-ping \
    rsyslog \
    htop \
    vim \
    xinit xorg \
    alsa-utils \
    sudo \
    --no-install-recommends

# Create user with home directory
useradd -m dogentu
# Add password for user
passwd dogentu
# Add to sudoers
usermod -aG sudo dogentu
# Add root password
passwd

hostname=fvm
hosts=$(cat <<EOF
127.0.0.1   localhost
127.0.0.1   $hostname
EOF
)
# Setup hostname
echo "$hostname" > /etc/hostname
echo "$hosts" > /etc/hosts

## Make X used by ‘anyuser’
echo "allowed_users=anybody" > /etc/X11/Xwrapper.config

# Configure timezone
dpkg-reconfigure tzdata

# Set up group for alsa
vim /etc/group
# Change 'audio:x:29' and 'video:x:44' to:
# audio:x:29:pulse,dogentu
# video:x:44:dogentu

# Set up group for fbterm
vim /etc/group

# Exit at this point from chroot:
# exit

# Unmount
sudo umount /mnt/tmpdir/proc
sudo umount /mnt/tmpdir/sys
sudo umount /mnt/tmpdir/dev/pts
sudo umount /mnt/tmpdir/dev

rootfs=rootfs.img
append="root=/dev/sda console=ttyS0"
kernel=vmlinuz-5.5.0-rc7+
initrd=initrd.img-5.5.0-rc7+

firectl \
    --kernel=../firecracker-guest/hello-vmlinux.bin \
    --root-drive=rootfs.img \
    --kernel-opts="root=/dev/sda ro console=ttyS0 noapic reboot=k panic=1 pci=off nomodules"
# qemu-system-x86_64 -m 1024M -nographic -kernel $kernel -append "$append" -hda $rootfs -net nic
