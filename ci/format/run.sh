#!/bin/bash

configurationFile="$REPO_ROOT/src/.clang-format"
filesToBeChecked=$(find "src" -type f \
  -name "*.c" -o \
  -name "*.cpp" -o \
  -name "*.h" -o \
  -name "*.hpp" \
  )

clang-format -style="file:$configurationFile" --dry-run --Werror ${filesToBeChecked[@]}
ret=$?

if [ $ret -ne 0 ]; then
  echo "Format check failed, configure your editor or fix it manually:"
  echo "scripts/dev.sh clang-format -style=\"file:.clang-format\" -i FILE_TO_FORMAT"
  echo "Example:"
  echo "scripts/dev.sh clang-format -style=\"file:.clang-format\" -i src/client/scene/SceneBase.hpp"
fi

exit $ret
