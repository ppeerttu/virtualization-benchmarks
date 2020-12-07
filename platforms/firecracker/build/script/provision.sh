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

apt-get update

apt-get install -y \
    iperf3 \
    sysbench
