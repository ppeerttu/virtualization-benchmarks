#! /bin/bash
# Sourced from: https://github.com/bkleiner/ubuntu-firecracker/blob/master/script/provision.sh
set -ex

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
    netplan.io \
    --no-install-recommends

useradd -m -p $(openssl passwd -1 $rootfs_password) $rootfs_username
usermod -aG sudo $rootfs_username
# passwd -d root

echo "$hostname" > /etc/hostname
echo "$hosts" > /etc/hosts

echo "allowed_users=anybody" > /etc/X11/Xwrapper.config

# cp /usr/share/zoneinfo/$rootfs_tz /etc/localtime

mkdir /etc/systemd/system/serial-getty@ttyS0.service.d/
cat <<EOF > /etc/systemd/system/serial-getty@ttyS0.service.d/autologin.conf
[Service]
ExecStart=
ExecStart=-/sbin/agetty --autologin root -o '-p -- \\u' --keep-baud 115200,38400,9600 %I $TERM
EOF

cat <<EOF > /etc/netplan/99_config.yaml
network:
  version: 2
  renderer: networkd
  ethernets:
    eth0:
      dhcp4: true
EOF

netplan generate
