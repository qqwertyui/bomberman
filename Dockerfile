FROM ubuntu:25.04

# install necessary tools & utilities
RUN apt-get update && apt-get install -y \
    git wget unzip \
    gcc g++ cmake make \
    xz-utils clang-format \
    clang-tidy nano gdb

# install protobuf
RUN apt-get install -y \
    protobuf-compiler \
    libprotobuf-dev

# install SFML
RUN apt-get install -y \
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

RUN wget https://github.com/SFML/SFML/releases/download/3.0.0/SFML-3.0.0-sources.zip
RUN /bin/bash -c "unzip SFML-3.0.0-sources.zip \
  && cd SFML-3.0.0 \
  && mkdir build \
  && cd build \
  && cmake .. \
  && make -j$(nproc) \
  && make install"

CMD ["bash"]
