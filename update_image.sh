#!/bin/bash

fuse-ext2 -o rw+ floppy.img ./floppy
sudo cp src/kernel floppy/kernel
diskutil unmount ./floppy
