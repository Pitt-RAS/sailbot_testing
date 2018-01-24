#!python2
import sys
#import numpy as np

#AppWindSpeed = input("Apparent Wind Speed: ")
#AppWindAngle = input("Apparent Wind Angle: ")
#AppWindDirection = input("Apparent Wind Direction: ")
#Heading = input("Heading: ")
#SpeedOverGround = input("Speed Over Ground: ")
#CourseOverGround = input("Course Over Ground: ")

def angleDifference(angle1, angle2):

	if (angle1 > angle2):
		angle = angle1 - angle2
	if (angle2 > angle1):
		angle = angle2 - angle1
		
	if (angle > 180):
		angle = angle - 180
		angle = 180 - angle
	
	return angle

while True:
	angle1 = int(input("Angle One: "))
	angle2 = int(input("Angle Two: "))
	print (angleDifference(angle1, angle2))


