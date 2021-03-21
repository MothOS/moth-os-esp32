#!/usr/bin/env bash

SDK_VER=$1
shift

USB_DEVICES=""
for device in $(ls /dev/ | grep "ttyUSB" | xargs -L1); do
  USB_DEVICES="$USB_DEVICES --device=/dev/$device"
done

excom="docker run --rm -it -v $PWD:/project -w /project -e UID="`id -u`" -e GID="`id -g`" $USB_DEVICES chromamaster/esp-idf-builder:v${SDK_VER} $@"
echo $excom
exec $excom
