FROM ubuntu:20.04

ENV TZ=America/New_York
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update \
 && apt-get -y upgrade \
 && apt-get clean \
 && apt-get install -y \
    curl \
    git \
    wget \
    python3 \
    build-essential \
    gcc-7 \
    g++-7 \
 && ln -sf /usr/bin/gcc-7 /usr/bin/gcc \
 && ln -sf /usr/bin/g++-7 /usr/bin/g++ \
 && curl https://cmake.org/files/v3.19/cmake-3.19.3-Linux-aarch64.tar.gz --output cmake-3.19.3-Linux-aarch64.tar.gz \
 && tar -xvf /cmake-3.19.3-Linux-aarch64.tar.gz \
 && ln -s /cmake-3.19.3-Linux-aarch64/bin/* /usr/local/bin \
 && rm /cmake-3.19.3-Linux-aarch64.tar.gz \
 && apt-get clean

ADD . /csc512_fall2022

WORKDIR /csc512_fall2022