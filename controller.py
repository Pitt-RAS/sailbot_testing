#Landsailer Control Script
#University of Pittsburgh
#Robotics and Automation Society
#Jeremy Manin, Controls and Actuators Lead
###USE WITH "get_input.sh" FOR CONTINUOUS INPUT###

import serial #pyserial module
import sys
import time

#initialize serial port at 9600 baud
try:
  ser = serial.Serial('/dev/cu.usbmodem1421', 9600)
except:
  print('Could not open serial port. Check USB.')
  exit(1)

#initialize monitor variables
rudder = 0;
sheets = 0;

#wait 5 seconds for serial to open
print('5')
time.sleep(1)
print('4')
time.sleep(1)
print('3')
time.sleep(1)
print('2')
time.sleep(1)
print('1')
time.sleep(1)
print('READY\n')

#',' = rudder left
#'.' = rudder right
#'h' = rudder home
#'s' = sheets out
#'w' = sheets in
#'q' = quit

#read in char from console
line = sys.stdin.read(1)
#read in newline char
sys.stdin.read(1)

while line!='q':
  if(line==',' and rudder<=177):
    rudder= rudder+3
  elif(line=='.' and rudder>=3):
    rudder= rudder-3
  elif(line=='h'):
    rudder= 90
#  elif(line=='s' and sheets<=INSERT MAX VALUE):
  elif(line=='s'):
    sheets= sheets+10
  elif(line=='w' and sheets>=10):
    sheets= sheets-10

  #print current status
  print('rudder: {0}, sheets: {1}'.format(str(rudder),str(sheets)))

  #write char to serial
  ser.write(line)

  line = sys.stdin.read(1)
  sys.stdin.read(1)

#close serial port
ser.close()
