#!/bin/bash
rm -r builf
mkdir build && cd build
qmake ../
make
make install
