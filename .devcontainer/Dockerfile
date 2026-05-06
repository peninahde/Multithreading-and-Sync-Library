FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    git \
    g++ \
    gcc \
    make \
    gdb \
    sudo \
    && apt-get clean

RUN if id -u 1000 >/dev/null 2>&1; then \
        OLD_USER=$(getent passwd 1000 | cut -d: -f1); \
        usermod -l vscode $OLD_USER; \
        groupmod -n vscode $OLD_USER; \
        usermod -d /home/vscode -m vscode; \
    else \
        groupadd --gid 1000 vscode; \
        useradd --uid 1000 --gid 1000 -m vscode; \
    fi; \
    echo "vscode ALL=(root) NOPASSWD:ALL" > /etc/sudoers.d/vscode \
    && chmod 0440 /etc/sudoers.d/vscode

RUN mkdir -p /home/vscode/.ssh && chown -R vscode:vscode /home/vscode

USER vscode
WORKDIR /home/vscode
ENV HOME=/home/vscode