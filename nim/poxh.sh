#!/bin/bash

PROJ_DIR="/tmp/nimpoxh"
COMPILE_NAME="nim-poxhash-runner"
RUNNER_PATH="nim/runner.nim"

if [[ "$COMPILE" = "1" ]] || [[ ! -f "$PROJ_DIR/$COMPILE_NAME" ]]; then
    rm -f $PROJ_DIR/$COMPILE_NAME
    mkdir -p $PROJ_DIR
    nim c -d:release  -o:$COMPILE_NAME --outdir:$PROJ_DIR $RUNNER_PATH &> /dev/null
fi


$PROJ_DIR/$COMPILE_NAME $@