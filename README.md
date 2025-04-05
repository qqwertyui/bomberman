## 1. Description
A work-in-progress, C++, SFML 3.0-based multiplayer implementation of popular bomberman game.

## 2. Build
```
git clone git@github.com:qqwertyui/bomberman.git
cd bomberman && bash scripts/dev.sh
mkdir build && cd build && cmake .. && make
```

## 3. Run (in docker container)
#### a) server
```
cd /workspace && build/src/server/bomberman-server
```

#### b) client
```
cd /workspace && build/src/client/bomberman-client
```
