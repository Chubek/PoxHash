#!/bin/bash

BASE_DIR="go"
PROJ_DIR="/tmp/gopoxh"
LIB_NAME="libpoxh"
RUNNER_NAME="runner.go"
LIBPOX_NAME="libpoxh.go"
COMPILE_NAME="go-poxhash-runner"
MOD_NAME="go.mod"
MAIN_NAME="main.go"

if [[ "$COMPILE" = "1" ]] || [[ ! -f "$PROJ_DIR/$COMPILE_NAME" ]]; then
    mkdir -p $PROJ_DIR $PROJ_DIR/$LIB_NAME
    cp $BASE_DIR/$RUNNER_NAME $PROJ_DIR/$MAIN_NAME
    cp $BASE_DIR/$LIBPOX_NAME $PROJ_DIR/$LIB_NAME/$LIBPOX_NAME
    rm -f $PROJ_DIR/$COMPILE_NAME
    rm -f $PROJ_DIR/$MOD_NAME
    cd $PROJ_DIR  && go mod init pox &> /dev/null && go mod tidy && go build -ldflags "-s -w" -o $PROJ_DIR/$COMPILE_NAME main.go &> /dev/null 
fi

$PROJ_DIR/$COMPILE_NAME $@
