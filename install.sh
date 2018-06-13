#!/bin/bash
mkdir build
cd build
Rscript ../load.R
cd ..
rmdir build
rm -f src/*.o
rm -f src/*.so
cd ..
rm tdatools*.tar.gz

