# Firecracker

This document describes how the author set up [Firecracker](https://github.com/firecracker-microvm/firecracker). Instructions were sourced from [Getting Started Guide](https://github.com/firecracker-microvm/firecracker/blob/master/docs/getting-started.md).

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
