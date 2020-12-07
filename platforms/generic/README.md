# generig

This directory contains generic scripts to install the benchmarking software on Ubuntu.

## Requirements

* Ubuntu Focal (20.04) installed

## Installing GitLab

1. Run `sudo ./gitlab/run_install`
2. Once the installation completes, setup GitLab root account and go to the Runners page
3. Register the gitlab runner with `sudo ./gitlab/run_register`, fill in proper config from the Runners page

## Installing sysbench

1. Run `sudo ./sysbench/run_install`

## Installinc iperf3

1. Run `sudo ./iperf3/run_install`
