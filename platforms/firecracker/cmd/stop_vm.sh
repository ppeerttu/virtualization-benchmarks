#!/bin/bash
#
# Required args:
#   - SOCKET - The Firecracker socket

curl --unix-socket $SOCKET -i \
  -X PUT 'http://localhost/actions'       \
  -H  'Accept: application/json'          \
  -H  'Content-Type: application/json'    \
  -d '{
      "action_type": "SendCtrlAltDel"
   }'
