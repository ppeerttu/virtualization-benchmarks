#!/bin/bash
#
# Required args:
#   - SOCKET - Firecracker socket
#   - KERNEL_PATH - Path of the kernel on host

curl --unix-socket $SOCKET -i \
    -X PUT 'http://localhost/boot-source'   \
    -H 'Accept: application/json'           \
    -H 'Content-Type: application/json'     \
    -d "{
        \"kernel_image_path\": \"${KERNEL_PATH}\",
        \"boot_args\": \"console=ttyS0 reboot=k panic=1 pci=off\"
    }"
