# Firecracker

This document describes how the author set up [Firecracker](https://github.com/firecracker-microvm/firecracker). Instructions were sourced from [Getting Started Guide](https://github.com/firecracker-microvm/firecracker/blob/master/docs/getting-started.md) and [firectl Getting Started on AWS](https://github.com/firecracker-microvm/firectl#getting-started-on-aws) guide. Networking is from [s8sg GitHub](https://gist.github.com/s8sg/1acbe50c0d2b9be304cf46fa1e832847).

KVM was [already installed](kvm.md) on the system.

```sh
#################################
# Pre-requisities
#################################

# KVM: Already installed

# Check Linux version > 4.14
$ uname -r
5.4.0-54-generic

# Grant read/write access
$ sudo setfacl -m u:${USER}:rw /dev/kvm

#################################
# Install Firecrackerf
#################################

# Get latest Firecracker version
$ latest=$(basename $(curl -fsSLI -o /dev/null -w  %{url_effective} https://github.com/firecracker-microvm/firecracker/releases/latest))
$ echo $latest
v0.23.0

# Check current directory
$ pwd
/home/perttu

# Download the binary
$ curl -LOJ https://github.com/firecracker-microvm/firecracker/releases/download/${latest}/firecracker-${latest}-$(uname -m)
# Rename to 'firecracker'
$ mv firecracker-${latest}-$(uname -m) firecracker
# Make executable
$ chmod +x firecracker
# Add to $PATH
$ sudo mv firecracker /usr/local/bin/firecracker

#################################
# Install firectl
#################################

# Download binary
$ curl -Lo firectl https://firectl-release.s3.amazonaws.com/firectl-v0.1.0
# Download SHA256 checksum
$ curl -Lo firectl.sha256 https://firectl-release.s3.amazonaws.com/firectl-v0.1.0.sha256
# Check the binary
$ sha256sum -c firectl.sha256
firectl: OK
# Make binary executable
$ chmod +x firectl
# Add to $PATH
$ sudo mv firectl /usr/local/bin/firectl

# Download kernel and rootfs
$ mkdir firecracker-guest
$ cd firecracker-guest
$ curl -LOJ https://s3.amazonaws.com/spec.ccfc.min/img/hello/kernel/hello-vmlinux.bin
$ curl -LOJ https://s3.amazonaws.com/spec.ccfc.min/img/hello/fsfiles/hello-rootfs.ext4

#################################
# Running a guest, using two shells
#################################

# First shell
$ rm -f /tmp/firecracker.socket
$ ./firecracker --api-sock /tmp/firecracker.socket

# Second shell
$ cd ~/firecracker-guest
# NOTE: Copy the scripts below into this folder
# E.g. Cntrl + C => nano set_kernel.sh => Cntrl + V => Cntrl + X
$ sh ./set_kernel.sh
HTTP/1.1 204
Server: Firecracker API
Connection: keep-alive

$ sh ./set_rootfs.sh
HTTP/1.1 204
Server: Firecracker API
Connection: keep-alive

$ sh ./launch_guest.sh
HTTP/1.1 204
Server: Firecracker API
Connection: keep-alive

# First shell you should see
Starting default runlevel

Welcome to Alpine Linux 3.8
Kernel 4.14.55-84.37.amzn2.x86_64 on an x86_64 (ttyS0)

localhost login:

# Login with root and root
login[855]: root login on 'ttyS0'
localhost:~#
# Shut down guest with reboot as Firecracker doesn't implement power management
localhost:~# reboot
PID1: Received "reboot" from FIFO...
localhost:~# Starting reboot runlevel
 * Unmounting loop devices
 * Unmounting filesystems
Sending the final term signal
Sending the final kill signal
[  279.672713] Unregister pv shared memory for cpu 0
[  279.674170] reboot: Restarting system
[  279.675205] reboot: machine restart
$

#################################
# Networking setup
#################################

# The br0 has been set up in KVM config

# Create a tap device & link with bridge
$ sudo ip tuntap add dev tap0 mode tap
$ sudo brctl addif br0 tap0
$ sudo ifconfig tap0 up
# Get MAC
$ ip a | grep -A1 tap0 | grep ether
    link/ether 32:76:c6:d0:d6:bd brd ff:ff:ff:ff:ff:ff

# Host
$ ifconfig
... br0 ...
      ... inet 192.168.1.82  netmask 255.255.255.0 ...
      ...
# This is 192.168.1.82/24

# Launch guest with new tap
$ firectl --kernel=hello-vmlinux.bin --root-drive=hello-rootfs.ext4 --tap-device=tap0/32:76:c6:d0:d6:bd
# Inside the guest
$ ifconfig eth0 up \
  && ip addr add dev eth0 192.168.1.83/16 \
  && ip route add default via 192.168.1.82 \
  && echo "nameserver 8.8.8.8" > /etc/resolv.conf




# We're going to use the docker0 interface for Firecracker guests
# by creating a new tap device and adding it to the bridge.

# ifconfig eth0 up && ip addr add dev eth0 172.17.0.3/16 && ip route
add default via 172.17.0.1 && echo "nameserver 8.8.8.8" > /etc/resolv.conf

# Create a tap device & link with bridge
$ sudo ip tuntap add dev tap0 mode tap
$ sudo brctl addif br0 tap0
$ sudo ifconfig tap0 up
# Get MAC
$ ip a | grep -A1 tap0 | grep ether
    link/ether 32:76:c6:d0:d6:bd brd ff:ff:ff:ff:ff:ff

# Host
$ ifconfig
... br0 ...
      ... inet 192.168.1.82  netmask 255.255.255.0 ...
      ...
# This is 192.168.1.82/24

# Launch guest with new tap
$ firectl --kernel=hello-vmlinux.bin --root-drive=hello-rootfs.ext4 --tap-device=tap0/32:76:c6:d0:d6:bd
# Inside the guest
$ ifconfig eth0 up \
  && ip addr add dev eth0 192.168.1.83/16 \
  && ip route add default via 192.168.1.82 \
  && echo "nameserver 8.8.8.8" > /etc/resolv.conf


```

## Scripts

Required scripts for launching the guest VM.

### set_kernel.sh

```bash
arch=`uname -m`
kernel_path=$(pwd)"/hello-vmlinux.bin"

if [ ${arch} = "x86_64" ]; then
    curl --unix-socket /tmp/firecracker.socket -i \
      -X PUT 'http://localhost/boot-source'   \
      -H 'Accept: application/json'           \
      -H 'Content-Type: application/json'     \
      -d "{
            \"kernel_image_path\": \"${kernel_path}\",
            \"boot_args\": \"console=ttyS0 reboot=k panic=1 pci=off\"
       }"
elif [ ${arch} = "aarch64" ]; then
    curl --unix-socket /tmp/firecracker.socket -i \
      -X PUT 'http://localhost/boot-source'   \
      -H 'Accept: application/json'           \
      -H 'Content-Type: application/json'     \
      -d "{
            \"kernel_image_path\": \"${kernel_path}\",
            \"boot_args\": \"keep_bootcon console=ttyS0 reboot=k panic=1 pci=off\"
       }"
else
    echo "Cannot run firecracker on $arch architecture!"
    exit 1
fi
```

### set_rootfs.sh

```bash
rootfs_path=$(pwd)"/hello-rootfs.ext4"
curl --unix-socket /tmp/firecracker.socket -i \
  -X PUT 'http://localhost/drives/rootfs' \
  -H 'Accept: application/json'           \
  -H 'Content-Type: application/json'     \
  -d "{
        \"drive_id\": \"rootfs\",
        \"path_on_host\": \"${rootfs_path}\",
        \"is_root_device\": true,
        \"is_read_only\": false
   }"
```

### launch_guest.sh

```bash
curl --unix-socket /tmp/firecracker.socket -i \
  -X PUT 'http://localhost/actions'       \
  -H  'Accept: application/json'          \
  -H  'Content-Type: application/json'    \
  -d '{
      "action_type": "InstanceStart"
   }'
```
