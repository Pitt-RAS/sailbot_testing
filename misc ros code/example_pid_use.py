#!/usr/bin/env python
#
# This is an example of how PID.py has been used for a class project
# Use a search tool to find references to "pid", most of this is junk
#
import roslib
import rospy
import cv2
import copy
import math
import PID
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
from geometry_msgs.msg import Twist

pid=PID.PID(0.3,0.0,0.0)
pid.setPoint(0)

colorImage = Image()
isColorImageReady = False
depthImage = Image()
isDepthImageReady = False;
pub = rospy.Publisher("kobuki_command", Twist, queue_size=10)

startedTurn = False

rightDistance1 = 0.0
rightDistance2 = 0.0
rightDistance3 = 0.0
leftDistance1 = 0.0
leftDistance2 = 0.0
leftDistance3 = 0.0
centerDistance1 = 0.0
centerDistance2 = 0.0
centerDistance3 = 0.0

turnSpeed = 0.0

rightDistance = 0.0
leftDistance = 0.0

measureY = 280
measureXRight = 600
measureXLeft = 40

currentCommand = Twist()
currentCommand.linear.x = 0.0
currentCommand.angular.z = 0.0

bridge = CvBridge()


def updateColorImage(data):
	global colorImage, isColorImageReady
	colorImage = data
	isColorImageReady = True

def updateDepthImage(data):
	global measureY, measureXRight, measureXLeft, rightDistance1, rightDistance2, rightDistance3, leftDistance1, leftDistance2, leftDistance3, isDepthImageReady, depthImage, bridge, centerDistance1, centerDistance2, centerDistance3
	depthImage = data
	isDepthImageReady = True
	
	try:
		depth = bridge.imgmsg_to_cv2(data, desired_encoding="passthrough")
	except CvBridgeError, e:
		print e
		print "depthImage"

	#camera: 6
	rightDistance1 = depth.item(measureY+20,measureXRight,0)
	rightDistance2 = depth.item(measureY,measureXRight,0)
	rightDistance3 = depth.item(measureY-20,measureXRight,0)

	leftDistance1 = depth.item(measureY+20,measureXLeft,0)
	leftDistance2 = depth.item(measureY,measureXLeft,0)
	leftDistance3 = depth.item(measureY-20,measureXLeft,0)
	
	centerDistance1 = depth.item(measureY+20,320,0)
	centerDistance2 = depth.item(measureY,320,0)
	centerDistance3 = depth.item(measureY-20,320,0)
	


def main():
	global measureY, measureXRight, measureXLeft, colorImage, isColorImageReady, depthImage, isDepthImageReady,  rightDistance1, rightDistance2, rightDistance3, leftDistance1, leftDistance2, leftDistance3, pub, currentCommand, rightDistance, leftDistance, pid, bridge, centerDistance1, centerDistance2, centerDistance3, startedTurn, turnSpeed
	rospy.init_node('showBlobs', anonymous=True)
	rospy.Subscriber("/camera/depth/image", Image, updateDepthImage, queue_size=10)
	rospy.Subscriber("/camera/rgb/image_color", Image, updateColorImage, queue_size=10)	   
	cv2.namedWindow("Blob Location")

	while not rospy.is_shutdown() and (not isDepthImageReady):
		pass

	while not rospy.is_shutdown():
		try:
			depth = bridge.imgmsg_to_cv2(depthImage, desired_encoding="passthrough")
		except CvBridgeError, e:
			print e
			print "depthImage"
			
		try:
			color_image = bridge.imgmsg_to_cv2(colorImage, "bgr8")
		except CvBridgeError, e:
			print e
			print "colorImage"

		#Calculate avg distances
		rightCount = 0
		if(not math.isnan(rightDistance1)):
			rightDistance += rightDistance1
			rightCount+=1
		if(not math.isnan(rightDistance2)):
			rightDistance += rightDistance2
			rightCount+=1
		if(not math.isnan(rightDistance3)):
			rightDistance += rightDistance3
			rightCount+=1
		
		#print("Right count:" + str(rightCount))
		#print("Right dist before:" + str(rightDistance))		
		if rightCount > 0:
			rightDistance = rightDistance/rightCount
		elif rightDistance > 1.5:
			rightDistance = 6
		else:
			rightDistance = 0.5
		#print("Right dist after:" + str(rightDistance))

		if rightDistance > 6.0:
			rightDistance = 6.0
		
		leftCount = 0
		if(not math.isnan(leftDistance1)):
			leftDistance += leftDistance1
			leftCount+=1
		if(not math.isnan(leftDistance2)):
			leftDistance += leftDistance2
			leftCount+=1
		if(not math.isnan(leftDistance3)):
			leftDistance += leftDistance3
			leftCount+=1
		
		#print("Left count:" + str(leftCount))
		#print("Left dist before:" + str(leftDistance))
		if leftCount > 0:
			leftDistance = leftDistance/leftCount
		elif leftDistance > 1.5:
			leftDistance = 6
		else:
			leftDistance = 0.5
		#print("Left dist after:" + str(leftDistance))			
		
		if leftDistance > 6.0:
			leftDistance = 6.0

		turnSpeed = pid.update(rightDistance - leftDistance)
		#turnSpeed = (leftDistance - rightDistance) * 0.2

		centerDistance = 0
		centerCount = 0
		if(not math.isnan(centerDistance1)):
			centerDistance += centerDistance1
			centerCount+=1
		if(not math.isnan(centerDistance2)):
			centerDistance += centerDistance2
			centerCount+=1
		if(not math.isnan(centerDistance3)):
			centerDistance += centerDistance3
			centerCount+=1

		if centerCount > 0:
			centerDistance = centerDistance/centerCount
		else:
			centerDistance = 7.25			

		if(centerDistance < 2.75):
#			startedTurn = True
#			if(turnSpeed < 0): #turning left
#				leftDistance = 5.9
#			elif(turnSpeed > 0): #turning right
#				rightDistance = 5.9

			#turnSpeed = (leftDistance - rightDistance) * 0.4

			if turnSpeed < -0.7:
				turnSpeed = -0.7
			if turnSpeed > 0.7:
				turnSpeed = 0.7		

			currentCommand.linear.x -= 0.01
			if(currentCommand.linear.x < 0.3):	
				currentCommand.linear.x = 0.3
#		elif(centerDistance > 2.5 and centerDistance < 7.25 and startedTurn == False):
#			if turnSpeed < -0.2:
#				turnSpeed = -0.2
#			if turnSpeed > 0.2:
#				turnSpeed = 0.2
#			currentCommand.linear.x -= 0.1
#			if(currentCommand.linear.x < 0.2):	
#				currentCommand.linear.x = 0.2
		elif(centerDistance < 7):
			if turnSpeed < -0.4:
				turnSpeed = -0.4
			if turnSpeed > 0.4:
				turnSpeed = 0.4

			currentCommand.linear.x -= 0.01
			if(currentCommand.linear.x < 0.6):	
				currentCommand.linear.x = 0.6
		else:
#			startedTurn = False
#			if turnSpeed < -0.35:
#				turnSpeed = -0.35
#			if turnSpeed > 0.35:
#				turnSpeed = 0.35
			currentCommand.linear.x += 0.02
			if(currentCommand.linear.x > 0.8):	
				currentCommand.linear.x = 0.8

		print(currentCommand.linear.x)		

		#Right location
		depthStr = "%.2f" % rightDistance

		cv2.rectangle(color_image, (measureXRight-10,measureY-20), (measureXRight+10,measureY+20), (0,255,0), 2)
		cv2.putText(color_image, depthStr, (measureXRight-30,measureY+10), cv2.FONT_HERSHEY_SIMPLEX, 1, (0,0,255), 2)

		#Left location
		depthStr = "%.2f" % leftDistance

		cv2.rectangle(color_image, (measureXLeft-10,measureY-20), (measureXLeft+10,measureY+20), (0,255,0), 2)
		cv2.putText(color_image, depthStr, (measureXLeft+15,measureY+10), cv2.FONT_HERSHEY_SIMPLEX, 1, (0,0,255), 2)

		#Center location
		depthStr = "%.2f" % centerDistance

		cv2.rectangle(color_image, (320-10,measureY-20), (320+10,measureY+20), (0,255,0), 2)
		cv2.putText(color_image, depthStr, (320+15,measureY+10), cv2.FONT_HERSHEY_SIMPLEX, 1, (255,0,0), 2)

		cv2.imshow("Color Image", color_image)
		cv2.waitKey(1)
		
		#print(turnSpeed)

		currentCommand.angular.z = turnSpeed
		
		pub.publish(currentCommand)

	cv2.destroyAllWindows()
`11` 
if __name__ == '__main__':
	main()
