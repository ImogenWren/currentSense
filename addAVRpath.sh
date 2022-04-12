#!/bin/sh
# addAVRpath.sh
# Navigate to home directory, then to this directory.
# Then add directory to path



cd /
cd /home/pi/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/bin
PATH="$(pwd):$PATH"
cd /
