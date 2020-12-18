# Docker platform

This directory contains required files to build the platform with Docker.

GitLab stuff sourced from [omnibus-gitlab](https://gitlab.com/gitlab-org/omnibus-gitlab/-/tree/master/docker) repository.

## Requirements

* Docker installed
* Linux / OS X

## Running the GitLab with containers

1. Change directory: `cd gitlab`
2. Build the GitLab docker image: `./build`
3. Run the image: `./run`
4. Run the runner image: `./run_runner`
5. Go to the GitLab admin panel, and fetch runner registartion details
6. Run the registration: `./register_runner`
7. Restart the runner if needed: `docker restart gitlab-runner`

## Running sysbench

1. Build an image with `./sysbench/build`
2. Run sysbench test with `./sysbench/run`
   * The default test is `cpu`, you can run any other test by passing the command argument e.g. `./sysbench/run sysbench fileio run --file-test-mode=seqwr`
