#!/bin/bash

PROJ_DIR="/tmp/cpoxh"
COMPILE_NAME="$USER-c-poxhrun"
FILE_PATH="c/runner.c"

if [[ "$COMPILE" = "1" ]] || [[ ! -f "$PROJ_DIR/$COMPILE_NAME" ]]; then
    rm -f $PROJ_DIR/$COMPILE_NAME
    mkdir -p $PROJ_DIR
    gcc -O2 $FILE_PATH -o $PROJ_DIR/$COMPILE_NAME
fi

$PROJ_DIR/$COMPILE_NAME $@