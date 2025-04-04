#!/bin/bash

rm "$REPO_ROOT/build" -rf
mkdir "$REPO_ROOT/build" && cd "$REPO_ROOT/build"

cmake ..
if [ $? -ne 0 ]; then
  echo "CMake generation failed"
  exit 1
fi

make -j$(nproc)
if [ $? -ne 0 ]; then
  echo "Compilaiton failed"
  exit 1
fi
