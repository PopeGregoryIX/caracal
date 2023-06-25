#!/bin/bash

# delete old image - if we fail, don't want to run an old image
rm ./images/caracal64.img

# build the kernel
cd kernel
make clean
cd ..