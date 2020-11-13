# Docker platform

This directory contains required files to build the platform with Docker.

GitLab stuff sourced from [omnibus-gitlab](https://gitlab.com/gitlab-org/omnibus-gitlab/-/tree/master/docker) repository.

## Requirements

* Docker installed
* Linux / OS X

## Running the GitLab with containers

1. Build the GitLab docker image: `./gitlab/build`
2. Run the image: `./gitlab/run`
3. Run the runner image: `./gitlab/run_runner`
4. Go to the GitLab admin panel, and fetch runner registartion details
5. Run the registration: `./gitlab/register_runner`
6. Restart the runner if needed: `docker restart gitlab-runner`

## Running sysbench

1. Build an image with `./sysbench/build`
2. Run sysbench test with `./sysbench/run`
   * The default test is `cpu`, you can run any other test by passing the command argument e.g. `./sysbench/run sysbench fileio run --file-test-mode=seqwr`
