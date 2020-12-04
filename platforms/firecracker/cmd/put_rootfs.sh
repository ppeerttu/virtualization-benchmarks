#!/bin/bash
#
# Required args:
#   - SOCKET - Firecracker socket
#   - ROOTFS_PATH - Path of the filesystem on host

curl --unix-socket $SOCKET -i \
  -X PUT 'http://localhost/drives/rootfs' \
  -H 'Accept: application/json'           \
  -H 'Content-Type: application/json'     \
  -d "{
        \"drive_id\": \"rootfs\",
        \"path_on_host\": \"${ROOTFS_PATH}\",
        \"is_root_device\": true,
        \"is_read_only\": false
   }"
