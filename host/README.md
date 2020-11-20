# host

_Scripts and instructions to setup the bare metal host along with guest systems._

## Installing the operating system

1. Go to [Ubuntu Server Download](https://ubuntu.com/download/server) page, and download `Ubuntu 20.04.1`
2. (Optional) Verify the installation file md5 checksum
3. Flash the installation `.iso` -file into your USB stick
   * [Bootable USB on Windows](https://ubuntu.com/tutorials/create-a-usb-stick-on-windows#1-overview)
   * [Bootable USB on Mac](https://ubuntu.com/tutorials/create-a-usb-stick-on-macos#1-overview)
   * [Bootable USB on Ubuntu](https://ubuntu.com/tutorials/create-a-usb-stick-on-ubuntu#1-overview)
4. Boot the bare metal host from the USB disk
5. Install the Ubuntu server
   1. Choose language
   2. Choose partitions
   3. Select Docker to be installed
   4. Choose SSH access according to your own preferences
6. Once installation is finished, login and update packages with `sudo apt update` and `sudo apt upgrade`
7. Restart, re-configure your BIOS to not boot from the USB drive

## Installing KVM

See [the docs](docs/kvm.md).

## Installing Firecracker

See [the docs](docs/firecracker.md).

## Installing gVisor

See [the docs](docs/gvisor.md).
