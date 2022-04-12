#!/bin/sh
# uploadNano.sh

# Script to compile then upload sketch to arduino nano using UART port

echo "starting compile"
arduino-cli compile --fqbn arduino:samd:nano_33_iot currentSense
echo "software compiled"
sleep 1
echo "Forcing Programming Mode for Nano 33 IoT"
python IoTreset.py
echo "Starting Upload"
arduino-cli upload --port /dev/ttyAMA0 --fqbn arduino:samd:nano_33_iot ~/currentSense
echo "Upload Completed"
sleep 1

