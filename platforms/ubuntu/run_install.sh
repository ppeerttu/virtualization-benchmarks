#!/bin/bash
#
# Install GitLab and GitLab Runner to the machine

GITLAB_URL="http://gitlab.custom.local"

# GitLab server
sudo apt-get update
sudo apt-get install -y curl openssh-server ca-certificates tzdata

curl https://packages.gitlab.com/install/repositories/gitlab/gitlab-ee/script.deb.sh | sudo bash

sudo EXTERNAL_URL=$GITLAB_URL apt-get install gitlab-ee


# GitLab Runner
curl -L https://packages.gitlab.com/install/repositories/runner/gitlab-runner/script.deb.sh | sudo bash
export GITLAB_RUNNER_DISABLE_SKEL=true; sudo -E apt-get install gitlab-runner
