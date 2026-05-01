FROM ubuntu:24.04

RUN apt-get update \
    && apt-get install -y git g++ gcc make gdb \
    && usermod -l vscode ubuntu \
    && usermod -d /home/vscode -m vscode \
    && groupmod -n vscode ubuntu \
    && apt-get clean

USER vscode
WORKDIR /home/vscode
ENV HOME=/home/vscode
