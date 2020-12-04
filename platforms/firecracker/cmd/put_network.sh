#!/bin/bash
#
# Required args:
#   - SOCKET - Firecracker socket
#   - DEVICE_MAC - Network device mac address
# Optional args:
#   - TAP_DEVICE - Network device name

curl --unix-socket $SOCKET -i \
  -X PUT 'http://localhost/network-interfaces/eth0' \
  -H 'Accept: application/json' \
  -H 'Content-Type: application/json' \
  -d "{
      \"iface_id\": \"eth0\",
      \"guest_mac\": \"$DEVICE_MAC\",
      \"host_dev_name\": \"${TAP_DEVICE:-tap0}\"
    }"
