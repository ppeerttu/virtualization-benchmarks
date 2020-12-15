# kvm

This directory contains scripts that are explained in steps at [host/docs/kvm.md](../../host/docs/kvm.md). The scripts will install you all the required packages and setup networking with host bridge, which allows the guest machine to be visible in the network just like any other machine.

## Steps

You have to run two scripts.

```bash
# Do basic setup - install required stuff for kvm
./install_kvm

# Reboot the system in between to apply some config changes
sudo reboot

# Configure the network
./install_kvm_network
```

After this you should be able to create guest VMs by running:

```bash
./install_kvm_guest
```

This launches a legacy installer, which will be deprecated. However, for this project it should be fine. Please make sure you remember to select `OpenSSH Server` as installed software in order to be able to connect to the server.
