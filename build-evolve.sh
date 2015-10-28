#!/bin/sh

set -xe
git submodule update --init --recursive
cd evolve && make dependencies && make build
