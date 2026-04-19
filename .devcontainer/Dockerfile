FROM ubuntu:24.04

RUN apt-get update \
    && apt-get install -y git g++ gcc make gdb \
    && apt-get clean

WORKDIR /root

CMD ["sleep infinity"]
