import serial
import sys
import time
ser = serial.Serial('/dev/cu.usbmodem1421', 9600)
time.sleep(5)
line = sys.stdin.read(1)
while line!='q':
  ser.write(line)
  line = sys.stdin.read(1)
ser.close()
