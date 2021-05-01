#!/bin/env bash

PROJECT_DIR=$(dirname "$0")

source "$PROJECT_DIR/venv/bin/activate"

cd "$PROJECT_DIR/src/rt_impl" || exit
python setup.py build -j"$(nproc)" --force
python setup.py install --force
