FROM ubuntu:20.04

RUN apt update && apt -y install iperf3

# TODO: Expose ports?

CMD [ "iperf3", "-s" ]
