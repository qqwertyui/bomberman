## 1. Description
A work-in-progress, C++, SFML 3.0-based multiplayer implementation of popular bomberman game.

## 2. Build
### a) download tools & libraries
```
sudo apt update
sudo apt install \
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
```

### b) compile
```
git clone git@github.com:qqwertyui/bomberman.git
cd bomberman && mkdir build && cd build && cmake .. && make
```

## 3. Run (on localhost)
#### a) server
```
cd bomberman/build && ./src/server/bomberman-server
```

#### b) client
```
cd bomberman/build && ./src/client/bomberman-client --assetsDir $(pwd)/../assets/ --serverPort 1111
```
