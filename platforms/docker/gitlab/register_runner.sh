#!/bin/zsh

docker run --rm -it \
    --network=host \
    -v gitlab-runner-config:/etc/gitlab-runner \
    gitlab/gitlab-runner:latest register
