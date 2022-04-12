#!/bin/sh
# uploadNano.sh

# Script to compile then upload sketch to arduino nano using UART port

echo "starting compile"
arduino-cli compile --fqbn arduino:avr:nano currentSense
echo "software compiled"
sleep 4
echo "Starting Upload"
arduino-cli upload --port /dev/ttyAMA0 =fqbn arduino:avr:nano ~/currentSense
echo "Upload Completed"
sleep 2

