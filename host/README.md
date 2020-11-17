# host

_Scripts and instructions to setup the bare metal host._

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

## Setting up KVM

Instructions from [Ubuntu KVM Home](https://help.ubuntu.com/community/KVM).

### Default installation

1. Install required packags: `sudo apt install qemu-kvm libvirt-daemon-system libvirt-clients bridge-utils`
2. Add user to required groups
   1. Run ```sudo adduser `id -un` libvirt```
   2. Run ```sudo adduser `id -un` kvm```
3. Check if works: `virsh list --all` - should return an empty table with no error messages

### Host networking

_Instructions courtecy of [Randal Kamradt Sr](https://levelup.gitconnected.com/how-to-setup-bridge-networking-with-kvm-on-ubuntu-20-04-9c560b3e3991)._

The default KVM installation includes a virtual network bridge that new guest machines will join. This allows the guest to access the outside world, but not outside world to access the guest. In order to be able to expose the guest machine to the outside network, we will setup a custom bridged network.


1. Install pacakges: `sudo apt install virtinst`
2. Disable network filtering for performance reasons
   1. Create a file called `/etc/sysctl.d/bridge.conf` with the following contents
        ```bash
        net.bridge.bridge-nf-call-ip6tables=0
        net.bridge.bridge-nf-call-iptables=0
        net.bridge.bridge-nf-call-arptables=0
        ```
   2. Create a file called `/etc/udev/rules.d/99-bridge.rules` with the following contents:
        ```bash
        ACTION=="add", SUBSYSTEM=="module", KERNEL=="br_netfilter", \           RUN+="/sbin/sysctl -p /etc/sysctl.d/bridge.conf"
        ```
   3. Reboot to take into effect: `sudo reboot`
3. Use `ip link` to see what the default KVM bridge looks like. Defaults created for KVM are `virbr0` and `virbr-nic`. Example output:
    ```bash
    1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    2: enp4s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP mode DEFAULT group default qlen 1000
        link/ether 00:50:8d:9f:7c:76 brd ff:ff:ff:ff:ff:ff
    3: virbr0: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state DOWN mode DEFAULT group default qlen 1000
        link/ether 52:54:00:19:74:fb brd ff:ff:ff:ff:ff:ff
    4: virbr0-nic: <BROADCAST,MULTICAST> mtu 1500 qdisc fq_codel master virbr0 state DOWN mode DEFAULT group default qlen 1000
        link/ether 52:54:00:19:74:fb brd ff:ff:ff:ff:ff:ff
    5: docker0: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state DOWN mode DEFAULT group default
        link/ether 02:42:5b:02:9b:a1 brd ff:ff:ff:ff:ff:ff
    ```
4. Destroy the default network bridges for the KVM
   1. Run `virsh net-destroy default`
   2. Run `virsh net-undefine default`
   3. Check the network interfaces again to verify `virbr0` etc are gone: `ip link`
5. Create the custom network bridge
   1. Create a backup of `/etc/netplan/00-installer-config.yaml`
   2. Edit the `/etc/netplan/00-installer-config.yaml` according to changes below
        ```yaml
        # This is the network config written by 'subiquity'
        network:
        ethernets:
            enp4s0:
            dhcp4: yes
        version: 2

        # == Old file above, updated file below == 

        # This is the network config written by 'subiquity'
        network:
        ethernets:
            enp4s0:
                dhcp4: false
                dhcp6: false
        bridges:
            br0:
                # Ethernet network interface
                interfaces: [enp4s0]
                # Public IP of the host - find out yours e.g. with `ip a`
                addresses: [192.168.1.82/24]
                # IP address of router
                gateway4: 192.168.1.1
                mtu: 1500
                nameservers:
                    addresses: [8.8.8.8,8.8.4.4]
                parameters:
                    stp: true
                    forward-delay: 4
                dhcp4: no
                dhcp6: no
        version: 2

        ```
   3. Run `sudo netplan apply` to apply changes
   4. Check that the changes look correct with `ip a`
        ```bash
        1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
            link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
            inet 127.0.0.1/8 scope host lo
            valid_lft forever preferred_lft forever
            inet6 ::1/128 scope host
            valid_lft forever preferred_lft forever
        2: enp4s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel master br0 state UP group default qlen 1000
            link/ether 00:50:8d:9f:7c:76 brd ff:ff:ff:ff:ff:ff
        5: docker0: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state DOWN group default
            link/ether 02:42:5b:02:9b:a1 brd ff:ff:ff:ff:ff:ff
            inet 172.17.0.1/16 brd 172.17.255.255 scope global docker0
            valid_lft forever preferred_lft forever
        6: br0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default qlen 1000
            link/ether 00:50:8d:9f:7c:76 brd ff:ff:ff:ff:ff:ff
            inet 192.168.1.82/24 brd 192.168.1.255 scope global br0
            valid_lft forever preferred_lft forever
            inet6 fe80::250:8dff:fe9f:7c76/64 scope link
            valid_lft forever preferred_lft forever
        ```
6. Configure KVM network
   1. Create a file called `host-bridge.xml` with the following contents
        ```xml
        <network>
            <name>host-bridge</name>
            <forward mode="bridge"/>
            <bridge name="br0"/>
        </network>
        ```
   2. Put that config into use with following commands
        ```bash
        virsh net-define host-bridge.xml
        virsh net-start host-bridge
        virsh net-autostart host-bridge
        ```
   3. Verify that the autostart is applied with: `virsh net-list --all`
        ```bash
        Name          State    Autostart   Persistent
        ------------------------------------------------
        host-bridge   active   yes         yes
        ```

### Launch the guest

Clone this git repo into your host and `cd` into it. We can launch a guest Ubuntu server with the following command.

```bash
./host/launch_guest
```

You might need to run the command as `sudo`.
