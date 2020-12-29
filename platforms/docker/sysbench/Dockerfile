FROM ubuntu:20.04

RUN apt update && apt -y install curl

RUN curl -s https://packagecloud.io/install/repositories/akopytov/sysbench/script.deb.sh | bash && apt -y install sysbench

# Prepare files for File I/O test
RUN sysbench fileio prepare --file-total-size=5G

CMD [ "sysbench", "cpu", "run" ]
