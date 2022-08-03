#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/alex/Desktop/Atomic/build/modules/glfw
  /opt/homebrew/Cellar/cmake/3.23.2/bin/cmake -P /Users/alex/Desktop/Atomic/build/modules/glfw/cmake_uninstall.cmake
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/alex/Desktop/Atomic/build/modules/glfw
  /opt/homebrew/Cellar/cmake/3.23.2/bin/cmake -P /Users/alex/Desktop/Atomic/build/modules/glfw/cmake_uninstall.cmake
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/alex/Desktop/Atomic/build/modules/glfw
  /opt/homebrew/Cellar/cmake/3.23.2/bin/cmake -P /Users/alex/Desktop/Atomic/build/modules/glfw/cmake_uninstall.cmake
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/alex/Desktop/Atomic/build/modules/glfw
  /opt/homebrew/Cellar/cmake/3.23.2/bin/cmake -P /Users/alex/Desktop/Atomic/build/modules/glfw/cmake_uninstall.cmake
fi

