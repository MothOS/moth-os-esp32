#!/bin/bash

SDK_VER=$1
shift

excom="docker run --rm -it -v $PWD:/project -w /project -e UID="`id -u`" -e GID="`id -g`" --device=/dev/ttyUSB0 --entrypoint ./scripts/entry.sh espressif/idf:v${SDK_VER} $@"
echo $excom
exec $excom
