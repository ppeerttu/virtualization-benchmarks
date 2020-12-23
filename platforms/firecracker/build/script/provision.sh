#! /bin/bash
set -ex

dpkg -i /root/linux*.deb

echo 'ubuntu-focal' > /etc/hostname
echo "allowed_users=anybody" > /etc/X11/Xwrapper.config
useradd -m -p $(openssl passwd -1 fire) fire
usermod -aG sudo fire
passwd -d root
# echo root:root | /usr/sbin/chpasswd
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
     dhcp4: no
     addresses: [172.16.0.2/24]
     gateway4: 172.16.0.1
     nameservers:
       addresses: [8.8.8.8,8.8.4.4]
EOF
netplan generate

echo "nameserver 8.8.8.8" > /etc/resolv.conf

# add-apt-repository universe
# add-apt-repository multiverse

cat <<EOF > /etc/apt/sources.list
# See http://help.ubuntu.com/community/UpgradeNotes for how to upgrade to
# newer versions of the distribution.
deb http://fi.archive.ubuntu.com/ubuntu focal main restricted
# deb-src http://fi.archive.ubuntu.com/ubuntu focal main restricted

## Major bug fix updates produced after the final release of the
## distribution.
deb http://fi.archive.ubuntu.com/ubuntu focal-updates main restricted
# deb-src http://fi.archive.ubuntu.com/ubuntu focal-updates main restricted

## N.B. software from this repository is ENTIRELY UNSUPPORTED by the Ubuntu
## team. Also, please note that software in universe WILL NOT receive any
## review or updates from the Ubuntu security team.
deb http://fi.archive.ubuntu.com/ubuntu focal universe
# deb-src http://fi.archive.ubuntu.com/ubuntu focal universe
deb http://fi.archive.ubuntu.com/ubuntu focal-updates universe
# deb-src http://fi.archive.ubuntu.com/ubuntu focal-updates universe

## N.B. software from this repository is ENTIRELY UNSUPPORTED by the Ubuntu
## team, and may not be under a free licence. Please satisfy yourself as to
## your rights to use the software. Also, please note that software in
## multiverse WILL NOT receive any review or updates from the Ubuntu
## security team.
deb http://fi.archive.ubuntu.com/ubuntu focal multiverse
# deb-src http://fi.archive.ubuntu.com/ubuntu focal multiverse
deb http://fi.archive.ubuntu.com/ubuntu focal-updates multiverse
# deb-src http://fi.archive.ubuntu.com/ubuntu focal-updates multiverse

## N.B. software from this repository may not have been tested as
## extensively as that contained in the main release, although it includes
## newer versions of some applications which may provide useful features.
## Also, please note that software in backports WILL NOT receive any review
## or updates from the Ubuntu security team.
deb http://fi.archive.ubuntu.com/ubuntu focal-backports main restricted universe multiverse
# deb-src http://fi.archive.ubuntu.com/ubuntu focal-backports main restricted universe multiverse

## Uncomment the following two lines to add software from Canonical's
## 'partner' repository.
## This software is not part of Ubuntu, but is offered by Canonical and the
## respective vendors as a service to Ubuntu users.
# deb http://archive.canonical.com/ubuntu focal partner
# deb-src http://archive.canonical.com/ubuntu focal partner

deb http://fi.archive.ubuntu.com/ubuntu focal-security main restricted
# deb-src http://fi.archive.ubuntu.com/ubuntu focal-security main restricted
deb http://fi.archive.ubuntu.com/ubuntu focal-security universe
# deb-src http://fi.archive.ubuntu.com/ubuntu focal-security universe
deb http://fi.archive.ubuntu.com/ubuntu focal-security multiverse
# deb-src http://fi.archive.ubuntu.com/ubuntu focal-security multiverse
EOF

apt-get update

curl -s https://packagecloud.io/install/repositories/akopytov/sysbench/script.deb.sh | bash

apt-get install -y \
    iperf3 \
    sysbench \
    git \
    mysql-server
