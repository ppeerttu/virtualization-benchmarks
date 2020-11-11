# Docker platform

This repository contains required files to build the platform with Docker.

Sourced from [omnibus-gitlab](https://gitlab.com/gitlab-org/omnibus-gitlab/-/tree/master/docker) repository.

## Requirements

* Docker installed
* Linux / OS X

## Running the GitLab with containers

1. Build the GitLab docker image: `docker build -t gitlab-custom-ee:latest .`
2. Run the image: `sh ./run.sh`
3. Run the runner image: `sh ./run_runner.sh`
4. Go to the GitLab admin panel, and fetch runner registartion details
5. Run the registration: `sh ./register_runner.sh`
6. Restart the runner: `docker restart gitlab-runner`

