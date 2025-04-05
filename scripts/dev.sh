#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
REPO_ROOT=$( realpath "$SCRIPT_DIR/.." )

docker build "$REPO_ROOT" -t bm-dev
if [ $? -ne 0 ]; then
  echo "Docker image build failed, check above error(s) for more details"
  exit
fi

[ -n "$DISPLAY" ] && xhost +

cmd="bash"
if [ $# -gt 0 ]; then
  cmd="$@"
fi

docker run -it --rm \
  -e DISPLAY=$DISPLAY \
  -u $(id -u):$(id -g) \
  -v /etc/passwd:/etc/passwd:ro \
  -v /etc/group:/etc/group:ro \
  -v /home/$USER:/home/$USER \
  -v $REPO_ROOT:/workspace \
  -v /tmp/.X11-unix:/tmp/.X11-unix:ro \
  --device /dev/input \
  --network host \
  -w /workspace \
  bm-dev $cmd

[ -n "$DISPLAY" ] && xhost -
