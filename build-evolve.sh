#!/bin/sh

set -xe
git submodule init && git submodule update --recursive
cd evolve
make dependencies
make build
