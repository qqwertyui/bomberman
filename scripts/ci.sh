#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
REPO_ROOT=$( realpath "$SCRIPT_DIR/.." )

readonly imageName="bm-dev"
readonly startupScriptName="run.sh"

while [ $# -gt 0 ]; do
  case "$1" in
      "setup")
        docker build . -t "$imageName"
        exit
        ;;

      "run")
        shift
        if [ $# -gt 0 ]; then
          jobName="$1"
          testList+=("$jobName")
        else
          for jobDirectory in $(find "$REPO_ROOT/ci" -mindepth 1 -type d); do
            jobName=$(basename $jobDirectory)
            testList+=("$jobName")
          done
        fi
        ;;
      *)
        echo "Unknown command"
        exit 1
        ;;
  esac
  shift
done

for jobName in ${testList[@]}; do
  echo "[$jobName] start"
  
  docker run -i \
    -e REPO_ROOT=/ci \
    -v $REPO_ROOT:/ci \
    -w/ci \
    "$imageName" \
    bash ci/$jobName/$startupScriptName

  if [ $? -ne 0 ]; then
    echo "[$jobName] failed"
    exit 1
  fi
  echo "[$jobName] ok"
done

