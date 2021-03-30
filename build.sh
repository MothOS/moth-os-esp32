#!/usr/bin/env bash

PROJECT_DIR="$(realpath $0 | xargs dirname)"
SDK_VER=$1
shift

USB_DEVICES=""
for device in $(ls /dev/ | grep "ttyUSB" | xargs -L1); do
  USB_DEVICES="$USB_DEVICES --device=/dev/$device"
done

excom="docker run --rm -it -v $PROJECT_DIR:$PROJECT_DIR -w $PWD -e UID="`id -u`" -e GID="`id -g`" $USB_DEVICES chromamaster/esp-idf-builder:v${SDK_VER} $@"
echo $excom
exec $excom
