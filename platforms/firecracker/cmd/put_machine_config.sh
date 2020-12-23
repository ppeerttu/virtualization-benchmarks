#!/bin/bash
#
# Required args:
#   - SOCKET - Firecracker socket
#   - GUEST_VCPU - Guest vCPU count
#   - GUEST_MEMORY - Guest RAM MiB

curl --unix-socket $SOCKET -i \
    -X PUT 'http://localhost/machine-config'   \
    -H 'Accept: application/json'           \
    -H 'Content-Type: application/json'     \
    -d "{
      \"vcpu_count\": $GUEST_VCPU,
      \"mem_size_mib\": $GUEST_MEMORY,
      \"ht_enabled\": false
    }"
