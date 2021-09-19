#!/bin/bash

mkdir -p images
rm -f images/*.iso
xorriso -as mkisofs -R -f -no-emul-boot -o images/caracal.iso iso

