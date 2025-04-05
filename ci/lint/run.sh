#!/bin/bash

configurationFile="$REPO_ROOT/src/.clang-tidy"

if [ ! -f "$REPO_ROOT/build/compile_commands.json" ]; then
  echo "No compile_commands.json found, aborting"
  exit 1
fi

cd "$REPO_ROOT/build"
run-clang-tidy -config-file="$configurationFile"
ret=$?

if [ $ret -ne 0 ]; then
  echo "Tidy check failed, check above errors and correct them accordingly"
fi

exit $ret
