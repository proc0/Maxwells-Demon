#!/usr/bin/env bash

command -v cmake >/dev/null 2>&1 || { echo >&2 "CMake is not installed or not in the PATH. Please install CMake and add it to your PATH."; exit 1; }

APP_NAME="${PWD##*/}"
PLATFORM=Desktop
BUILD_TYPE=Debug
BUILD_PATH=build
SHOULD_RUN=false
GENERATOR="Unix Makefiles"

for arg in "$@"
do
  if [ "$arg" == "--web" ]; then
    command -v em++ >/dev/null 2>&1 || { echo >&2 "Emscripten is not installed or not in the PATH. Please install Emscripten to build for Web."; exit 1; }
    PLATFORM=Web
    BUILD_PATH=$BUILD_PATH-web
    shift
  elif [ "$arg" == "--release" ]; then
    PLATFORM=Release
  elif [ "$arg" == "--run" ]; then
    SHOULD_RUN=true
  fi
done

# Configure
if [ ! -d "$BUILD_PATH" ]; then
  if [ "$PLATFORM" = "Web" ]; then
    emcmake cmake -S . -B $BUILD_PATH -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DPLATFORM=$PLATFORM
  else
    cmake -S . -B $BUILD_PATH -G "$GENERATOR" -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DPLATFORM=$PLATFORM
  fi
fi

if [ $? ! -eq 0 ]; then
  echo >&2 "CONFIGURE ERROR"
  exit 1
fi

# Generate
cmake $BUILD_PATH -DPLATFORM=$PLATFORM -DCMAKE_BUILD_TYPE=$BUILD_TYPE

if [ $? ! -eq 0 ]; then
  echo >&2 "GENERATE ERROR"
  exit 1
fi

# Build
cmake --build $BUILD_PATH "$@"

if [ $? ! -eq 0 ]; then
  echo >&2 "BUILD ERROR"
  exit 1
fi

# Install
# cmake --install $BUILD_PATH

# Run
if [[ "$SHOULD_RUN" == "true" ]]; then
  if [[ "$PLATFORM" == "Web" ]]; then
    APP_PATH="$BUILD_PATH/$APP_NAME.html"
    if [ ! -f "$APP_PATH" ]; then
      echo >&2 "LAUNCH ERROR: $APP_PATH not found."
      exit 1
    fi

    emrun "$APP_PATH"
  else
    APP_PATH="./$BUILD_PATH/$APP_NAME"
    if [ ! -f "$APP_PATH" ]; then
      echo >&2 "LAUNCH ERROR: $APP_PATH not found."
      exit 1
    fi

    eval "$APP_PATH"
  fi
fi