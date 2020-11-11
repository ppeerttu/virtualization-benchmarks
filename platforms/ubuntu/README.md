# ubuntu

This directory contains required scripts to setup Ubuntu platform with required software.

## Requirements

* Ubuntu installed (20.04)
  * Can be virtual machine

## Installing GitLab

1. Run `sudo ./run_install.sh`
2. Once the installation completes, setup GitLab root account and go to the Runners page
2. Register the gitlab runner with `sudo ./run_register.sh`, fill in proper config from the Runners page
