# virtualization-benchmarks

_Benchmarking tools and related resources for different types of virtualization._

## What

This repository contains scripts for testing several different types of virtualization.

### Virtualization methods

* KVM
* Firecracker
* Docker
* Docker with gVisor
* No virtualization (bare metal)

### Tests

* CPU - sysbench
* Network - iperf3
* Filesystem - sysbench
* Relational database - sysbench
* End-to-end CI pipeline - GitLab as a platform, `ab` as HTTP benchmarking client

## Why

This benchmarking is done as a part of an empirical research for a Masters Thesis at the University of Oulu, Finland. The objective of the research is to find the performance differences between different types of virtualization, especially those types that are used in the context of serverless computing by AWS and Google Cloud.

## How

Due to heavy resource constraints (no funding, small research team), this benchmarking is done on very modest desktop computer hardware. More on that later.
