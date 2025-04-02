FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    git jq wget \
    xz-utils clang-format \
    gcc g++ cmake \
    make protobuf-compiler \
    libprotobuf-dev \
    libxrandr-dev \
    libxcursor-dev \
    libxi-dev \
    libudev-dev \
    libfreetype-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev \
    libfreetype-dev

RUN mkdir /workspace
WORKDIR /workspace

ENTRYPOINT ["bash"]
