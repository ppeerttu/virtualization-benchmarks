# ubuntu

This directory contains required scripts to setup Ubuntu platform with required software.

## Requirements

* Ubuntu installed (20.04)
  * Can be virtual machine

## Installing GitLab

1. Run `sudo ./gitlab/run_install`
2. Once the installation completes, setup GitLab root account and go to the Runners page
3. Register the gitlab runner with `sudo ./gitlab/run_register`, fill in proper config from the Runners page

# Installing sysbench

1. Run `sudo ./sysbench/run_install`
