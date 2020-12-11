# Benchmarking tests

Benchmarking tests are described in detail here.

- [Benchmarking tests](#benchmarking-tests)
  - [Setup](#setup)
  - [Structure](#structure)
  - [Tests](#tests)
    - [Boot](#boot)
    - [CPU](#cpu)
    - [Database](#database)
    - [File I/O](#file-io)
    - [Network](#network)
    - [Case app - HTTP benchmarking](#case-app---http-benchmarking)
    - [Case app - pipeline turnover time](#case-app---pipeline-turnover-time)

## Setup

Please see the [platforms](../platforms/README.md) for the system setup.

## Structure

Planned structure for conducting the tests. The order of running each test is always the same:

1. Bare metal
2. KVM
3. Firecracker
4. Docker
5. gVisor

The tests are run in the following structure.

| #   | Test    | Platform    | Concurrency level | Repeating times |
|---|---|---|---|---|
| 1 | Boot  | -  |  - | 10 |
| 2. | CPU  | -  | -  | 10 |
| 3. | File I/O  | -  | -  | 10 |
| 4. | Database | - | 10, 30, 60, 90, 120, 150, 180, 200 | 2 (each concurrency level) |
| 5. | Network | - | - | 10 |
| 6. | Case app - HTTP | - | 10, 30, 60, 90, 120, 150, 180, 200 | 2 (each concurrency level) |
| 7. | Case app - pipeline turonver | - | - | 10 |

Since each test set will be run for all platforms, total count of tests per platform will be 82 and thus the total count of tests will be 410.

## Tests

Invdividual tests are described here.

### Boot

The purpose of the booting test is to find out how quickly and reliably the machine can launch up from an existing image. The motivation behind this comes from basic requirements of serverless computing, where machine instances are short-living and launched on-demand and thus need to be able to ready to serve from 0 to 100 in matter of hundreads of milliseconds.

**Measurement feature**: Time to boot up

**Feature metric**: Seconds

**Tools**: `ping`, `time`

| Machine   | Test description  | Measurement   | Additional notes  |
|----|----|----|---|
| Metal | The raw metal instance is manually turned on by pressing the physical "Power on" -button. | Script that is polling the the machine IP with `ping` -command. We use the `time` command to measure the actual time. | The script has to be started at the same time as the machine is booted. This is the most unreliable method in this test, but with careful execution it is possible to reach decent accuracy on results. |
| KVM   | The instance is launched by issuing `virsh start <kvm-instance>`. | Similar as with the metal instance - script is polling the guest IP with `ping` -command, and `time` will return as the complete time. | This method is fully automated with scripts. |
| Firecracker   | The instance is launched with custom script. | Similar as above. | Same as above. |
| Docker        | The instance is launched with `docker start <container>` command. | The instance is considered to be launched when the `docker start` -call returns. | Same as above. |
| gVisor        | Same as above     | Same as above | Same as above. |

### CPU

The purpose of this test is to measure the performance of the platform under generic computing-intensive tasks. This test is conducted with `sysbench`, which contains a benchmark for calculating prime numbers.

**Measurement feature**: CPU performance

**Feature metric**: TBD

**Tools**: [sysbench][sysbench-site]

| Machine   | Test description  | Measurement   | Additional notes  |
|----|----|----|---|
| Metal | The test is run directly on the host. |   |   |
| KVM   | The test is run on the guest over SSH. |  |   |
| Firecracker   | The test is run on the guest over SSH. |   |  |
| Docker        | The test is run on custom-built container with `docker run` -command. |  |   |
| gVisor        | The test is run on custom-built container with `docker run` -command. |  |   |

### Database

The purpose of this test is to benchmark the platform performance under commonly used application stress. The database test contains a series of OLTP-like queries with different concurrency levels against a MySQL instance. From the set of available test suites, we run [oltp_read_only](https://github.com/akopytov/sysbench/blob/master/src/lua/oltp_read_only.lua), [oltp_write_only](https://github.com/akopytov/sysbench/blob/master/src/lua/oltp_write_only.lua) and [oltp_read_write](https://github.com/akopytov/sysbench/blob/master/src/lua/oltp_read_write.lua).

**Measurement feature**: CPU performance

**Feature metric**: Transactions per second

**Tools**: [sysbench][sysbench-site]

| Machine   | Test description  | Measurement   | Additional notes  |
|----|----|----|---|
| Metal | The MySQL server is running on the host, while the sysbench client is running on another machine. | The sysbench provides results for us. |   |
| KVM   | Similar as with the metal host. Guest VM is visible in the local network. | Same as above |   |
| Firecracker   | Similarly as above, except the guest VM is not visible to the local network. We use custom port forwarding from bare metal host. |   |  |
| Docker        | MySQL server running in container on the metal host. The server is accessed from another machine by using Docker port forwarding on the bare metal host. | Same as above |   |
| gVisor        | Same as above. | Same as above |   |

### File I/O

The purpose of this test is to measure the performance of the platform under generic filesystem intensive tasks. This test is conducted with `sysbench`. 

Available `fileio` tests are `seqwr`, `seqrewr`, `seqrd`, `rndrd`, `rndwr` and `rndrw`. In this benchmark, we conduct sequential read (`seqrd`), sequential write (`seqwr`) and random read write (`rndrw`). We use the default sysbench fileio test parameters for conducting the tests.

**Measurement feature**: Filesystem I/O throughput

**Feature metric**: Read MiB/s, write MiB/s

**Tools**: [sysbench][sysbench-site]

| Machine   | Test description  | Measurement   | Additional notes  |
|----|----|----|---|
| Metal | The test is run directly on the host. | The `sysbench` tool provides reuslts for us. |    |
| KVM   | The test is run on the guest over SSH. | The `sysbench` tool provides reuslts for us. |   |
| Firecracker   | The test is run on the guest over SSH. | The `sysbench` tool provides reuslts for us.  |  |
| Docker        | The test is run on custom-built container with `docker run` -command. | The `sysbench` tool provides reuslts for us. |   |
| gVisor        | The test is run on custom-built container with `docker run` -command. | The `sysbench` tool provides reuslts for us. |   |

### Network

The purpose of this test is to measure the network bandwidth on each of the platforms. The test will be conducted with `iperf3`.

**Measurement feature**: Network throughput

**Feature metric**: Received bits/s, sent bits/s

**Tools**: [iperf3][iperf-site]

| Machine   | Test description  | Measurement   | Additional notes  |
|----|----|----|---|
| Metal | The iperf3 server is launched on the host, and the test is run from another machine which connects to the server. | The `iperf3` tool provides results for us. |    |
| KVM   | Same as above, but the server is launched on the guest VM. | Same as above. |   |
| Firecracker   | Same as above. This requires custom port forwarding in the host. | Same as above.  |  |
| Docker        | Container running the iperf3 server is launched on the host, and Docker port forwarding used to make it reachable to other machine. | Same as above. |   |
| gVisor        | Same as above. | Same as above. |   |

### Case app - HTTP benchmarking

The purpose of this test is to measure the performance of the platform under generic application load. The application used in this case is a GitLab, and it is being invoked over HTTP with `ab` benchmarking tool under different concurrency levels.

**Measurement feature**: Generic performance

**Feature metric**: Req / s, mean time (ms) / req

**Tools**: [ab][ab-site]

| Machine   | Test description  | Measurement   | Additional notes  |
|----|----|----|---|
| Metal | The GitLab server is running on the host, and the `ab` is invoked from another machine. | The `ab` provides results for us. |    |
| KVM   | Same as above, but the GitLab server is launched on the guest VM. | Same as above. |   |
| Firecracker   | Same as above. This requires custom port forwarding in the host. | Same as above.  |  |
| Docker        | Container running the GitLab server is launched on the host, and Docker port forwarding used to make it reachable to other machine. | Same as above. |   |
| gVisor        | Same as above. | Same as above. |   |

### Case app - pipeline turnover time

The purpose of this test is to measure the performance of the platform under generic application load. The application used in this case is a GitLab, and it is being invoked over HTTP with `ab` benchmarking tool under different concurrency levels.

**Measurement feature**: Generic performance

**Feature metric**: Req / s, mean time (ms) / req

**Tools**: [ab][ab-site]

| Machine   | Test description  | Measurement   | Additional notes  |
|----|----|----|---|
| Metal | The GitLab server is running on the host, and the `ab` is invoked from another machine. | The `ab` provides results for us. |    |
| KVM   | Same as above, but the GitLab server is launched on the guest VM. | Same as above. |   |
| Firecracker   | Same as above. This requires custom port forwarding in the host. | Same as above.  |  |
| Docker        | Container running the GitLab server is launched on the host, and Docker port forwarding used to make it reachable to other machine. | Same as above. |   |
| gVisor        | Same as above. | Same as above. |   |

[sysbench-site]:https://github.com/akopytov/sysbench
[iperf-site]:https://iperf.fr
[ab-site]:https://httpd.apache.org/docs/2.4/programs/ab.html

