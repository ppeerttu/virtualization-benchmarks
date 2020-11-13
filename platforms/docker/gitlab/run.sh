#!/bin/zsh

docker run --detach \
    --hostname gitlab.custom.local \
    --publish 443:443 --publish 80:80 --publish 22:22 \
    --name gitlab \
    --restart always \
    gitlab-custom-ee:latest
