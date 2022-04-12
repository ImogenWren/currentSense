
#!/usr/bin/python

import RPi.GPIO as GPIO
import sys, os, re, time, fcntl
import errno

fd = sys.stdin.fileno()
fl = fcntl.fcntl(fd, fcntl.F_GETFL)
fcntl.fcntl(fd, fcntl.F_SETFL, fl | os.O_NONBLOCK)

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
dtr = re.compile('.+TIOCM_DTR.+')
start = time.time()
pin = 18

def reset():
  GPIO.setup(pin, GPIO.OUT)
  GPIO.output(pin, GPIO.HIGH)
  time.sleep(0.32)
  GPIO.output(pin, GPIO.LOW)
  time.sleep(0.5)
  GPIO.output(pin, GPIO.HIGH)
  time.sleep(0.32)
  GPIO.output(pin, GPIO.LOW)
  time.sleep(1)


reset()
print "avrdude-original: Using autoreset DTR on GPIO Pin " +str(pin)
GPIO.cleanup()
exit

