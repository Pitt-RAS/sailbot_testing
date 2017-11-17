#!/usr/bin/env python
#
import roslib
import rospy
import PID

pid=PID.PID(0.45,0.2,0.1)
pid.setPoint(0)
currentHeading = 0
goal = 0
pub = rospy.Publisher("serial_command", Char, queue_size=10)

def updateTarget(data):
	global goal
	goal = data

def updateHeading(data):
	global currentHeading
	currentHeading = data

def main():
	global pub, pid, currentHeading
	rospy.Subscriber("/heading/goal", Float32, updateTarget, queue_size=10)
	rospy.Subscriber("/heading/current", Float32, updateHeading, queue_size=10)
	
	while not rospy.is_shutdown():
		angleDiff = goal - currentHeading
		if(abs(current) > 180):
			if (current < 180) angleDiff = angleDiff - 360
			else angleDiff = angleDiff + 360
		
		sleepAmount = pid.update(angleDiff)
		rospy.sleep(sleepAmount)
		
		if(angleDiff > 0.001):
			pub.publish("<")
		elif(angleDiff < -0.001):
			pub.publish(">")
			
if __name__ == '__main__':
	main()
	