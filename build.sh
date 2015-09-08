#!/bin/bash

BUILD_DIR=build
INSTALL_PREFIX=${2:-/mnt/neon}
export PREFIX=$INSTALL_PREFIX

cabal exec -- runhaskell Make.hs ${1:-install}
