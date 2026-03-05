FROM ubuntu:22.04

# timezone
RUN apt-get update \
    && apt install -y tzdata \
    && apt-get clean && rm -rf /var/lib/apt/lists/* \
    && echo "Asia/Tokyo" >/etc/timezone

# create user
ARG USER
ARG UID
ARG GID
RUN apt-get update \
    && apt-get install -y sudo \
    && echo "$USER ALL=(ALL) NOPASSWD:ALL" >>/etc/sudoers.d/ALL \
    && groupadd \
        --gid $GID \
        $USER \
    && useradd \
        --uid $UID \
        --gid $GID \
        --home-dir /home/$USER \
        --create-home \
        --shell /bin/bash \
        $USER

USER $USER

RUN sudo apt-get update && sudo apt-get install -y \
        git \
        gnupg2 \
    && sudo apt-get clean \
    && sudo rm -rf /var/lib/apt/lists/*

RUN sudo apt-get update && sudo apt-get install -y \
        libgl1-mesa-dev \
        xorg-dev \
    && sudo apt-get clean \
    && sudo rm -rf /var/lib/apt/lists/*

RUN sudo apt-get update && sudo apt-get install -y \
        cmake \
        g++ \
        make \
        gdb \
        curl zip unzip tar \
    && sudo apt-get clean \
    && sudo rm -rf /var/lib/apt/lists/*

RUN sudo apt-get update && sudo apt-get install -y \
        libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-net-dev libsdl2-ttf-dev \
        libsoil-dev \
        libglew-dev libgl1-mesa-dev xorg-dev libglu1-mesa-dev \
        libopenal-dev libalut-dev libpulse0 libpulse-dev \
        rapidjson-dev \
        # mingw-w64 gcc-mingw-w64 g++-mingw-w64 \
        # python3 python3-jinja2 \
        # autoconf libtool \
    && sudo apt-get clean \
    && sudo rm -rf /var/lib/apt/lists/*
