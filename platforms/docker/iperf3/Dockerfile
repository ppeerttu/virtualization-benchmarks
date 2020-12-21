FROM ubuntu:20.04

RUN apt update && apt -y install iperf3

RUN mkdir -p /var/logs

CMD [ "iperf3", "-s" ]
