#!/usr/bin/env bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

docker build "$SCRIPT_DIR" -t bomberman-dev
if [ $? -ne 0 ]; then
  echo "Docker image build failed, check above error(s) for more details"
  exit
fi

[ -n "$DISPLAY" ] && xhost +

docker run -it --rm \
  -e DISPLAY=$DISPLAY \
  -u $(id -u):$(id -g) \
  -v /etc/passwd:/etc/passwd:ro \
  -v /etc/group:/etc/group:ro \
  -v /home:/home \
  -v $SCRIPT_DIR:/workspace \
  -v /tmp/.X11-unix:/tmp/.X11-unix:ro \
  --device /dev/input \
  --network host \
  bomberman-dev 

if [ $? -ne 0 ]; then
  echo "Unexpected problem occured during docker image execution, check above error(s) for more details"
  exit
fi

[ -n "$DISPLAY" ] && xhost -
