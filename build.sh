#!/bin/bash

BUILD_DIR=build
INSTALL_PREFIX=${2:-/mnt/neon}

case $1 in
    clean)
	# cleaning
	ABS_PATH_DEL=$(readlink -e $BUILD_DIR)
	echo "Deleting $ABS_PATH_DEL..."
	rm -r -I $BUILD_DIR/*
	;;
    build)
	# build only
	mkdir -p $BUILD_DIR
	pushd $BUILD_DIR
        cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX && make
	popd
	;;
    install)
	# install only
	pushd $BUILD_DIR
	make install
	popd
	;;
    *)
	# build and install
	mkdir -p $BUILD_DIR
	echo "Building and installing..."
	pushd $BUILD_DIR
        cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX && make && make install
	popd
	sync
esac
