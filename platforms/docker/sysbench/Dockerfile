FROM ubuntu:20.04

RUN apt update && apt -y install curl

RUN curl -s https://packagecloud.io/install/repositories/akopytov/sysbench/script.deb.sh | bash && apt -y install sysbench

RUN sysbench fileio prepare

CMD [ "sysbench", "cpu", "run" ]
