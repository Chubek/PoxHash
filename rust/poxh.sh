#!/bin/bash

PROJ_DIR="/tmp/rustpoxh"
COMPILE_NAME_EXEC="$USER-rs-poxhrun"
COMPILE_NAME_RLIB="libpoxh.rlib"
CRATE_NAME="libpoxh"
BASE_DIR="rust"
LIBPOX_NAME="libpoxh.rs"
RUNNER_NAME="runner.rs"

if [[ "$COMPILE" = "1" ]] || [[ ! -f "$PROJ_DIR/$COMPILE_NAME_EXEC" ]]; then
    rm -f $PROJ_DIR/$COMPILE_NAME_EXEC $PROJ_DIR/$COMPILE_NAME_RLIB
    mkdir -p $PROJ_DIR
    rustc $BASE_DIR/$LIBPOX_NAME -O --crate-type lib --crate-name $CRATE_NAME -o $PROJ_DIR/$COMPILE_NAME_RLIB
    rustc $BASE_DIR/$RUNNER_NAME -O --extern $CRATE_NAME=$PROJ_DIR/$COMPILE_NAME_RLIB -o $PROJ_DIR/$COMPILE_NAME_EXEC 
fi

$PROJ_DIR/$COMPILE_NAME_EXEC $@