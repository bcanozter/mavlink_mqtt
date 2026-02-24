FROM ubuntu:22.04

WORKDIR /home/
RUN apt-get update && apt-get install -y \
    software-properties-common \
    python3 \
    python3-pip


COPY . /app