# Firecracker

Firecracker setup for the benchmarking purposes. Build scripts with Docker courtesy of [bkleiner](https://github.com/bkleiner/ubuntu-firecracker).

## Requirements

See the [host](../../host/README.md) machine. You'll need Docker installed in case you want to build the kernel and system image with these scripts.

## Installation

You can setup the platform by using the script in this folder.

**Disclaimer**: These scripts are prepared for a custom machine setup. It is very likely that some of the configurations, such as network interface names, IP addresses or such won't work in other machine setups.

1. Run `./setup_environment`, which will
   1. Download the `firecracker` and `jailer` binaries for you
   2. Build a Linux kernel and Ubuntu system image for you
   3. Install all of them in `~/.firecracker` -directory
2. Run `./setup_network`, which will
   1. Setup a tap device for your guest
   2. Configure `iptables` for port forwarding
