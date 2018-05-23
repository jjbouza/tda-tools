#!/bin/bash
mkdir build
cd build
Rscript ../load.R
cd ..
rmdir build
cd ..
rm tdatools*.tar.gz

