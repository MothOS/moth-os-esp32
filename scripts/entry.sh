#!/bin/bash
set -e

. $IDF_PATH/export.sh

COMMAND=$1.sh

if [ -e ./scripts/$COMMAND ]; then
    shift
    ./scripts/$COMMAND "$@"
else
    "$@"
fi

chown -R $UID:$GID .
