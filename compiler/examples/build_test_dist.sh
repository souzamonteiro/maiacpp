#!/bin/bash
# Build/test script for compiler/examples/test.cpp
# Run from the project root: bash compiler/examples/build_test_dist.sh

set -e

ROOT_DIR=../..
cd $ROOT_DIR/compiler/examples

echo "==> webcpp: compile"
$ROOT_DIR/bin/webcpp.sh $ROOT_DIR/compiler/examples/test.cpp > test.c

echo "==> webcpp: create dist (browser + node)"
$ROOT_DIR/bin/webcpp.sh $ROOT_DIR/compiler/examples/test.cpp --dist --out-dir dist --name test

echo "==> dist node runner"
bash dist/node-runner.sh

echo "==> All steps OK"
