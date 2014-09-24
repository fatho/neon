#!/bin/bash

if (( $# < 2 || $# > 3 )); then
    echo "Usage: mount-image.sh <file> <mountpoint> [offset]"
    exit -1
fi

IMGFILE=$1
MOUNTPOINT=$2

if (($# == 3)); then
  OFFSET=$3
else
  OFFSET=1048576
fi

if [ ! -f "$IMGFILE" ]; then
    echo "$IMGFILE does not exist"
    exit -1
fi
if [ ! -d "$MOUNTPOINT" ]; then
    echo "$MOUNTPOINT does not exist"
    exit -1
fi

sudo mount -o rw,sync,loop,offset=$OFFSET "$IMGFILE" "$MOUNTPOINT"