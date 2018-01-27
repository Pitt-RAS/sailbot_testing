#!python2
import sys
import numpy as np

#AppWindSpeed = input("Apparent Wind Speed: ")
#AppWindAngle = input("Apparent Wind Angle: ")
#AppWindDirection = input("Apparent Wind Direction: ")
#Heading = input("Heading: ")
#SpeedOverGround = input("Speed Over Ground: ")
#CourseOverGround = input("Course Over Ground: ")

def angle_difference(angle1, angle2):

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
    print (angle_difference(angle1, angle2))

#Use the Law of Cosines to find the magnitude of the True Speed Vector

def find_true_speed(angle, apparent_wind_speed, speed_over_ground):
    
    #bc_squared = b^2 + c^2
    bc_squared = (apparent_wind_speed*apparent_wind_speed) + (speed_over_ground*speed_over_ground) 
    
    #a_squared = bc_squared - 2*a*b*cos(angle)
    opposite_squared = bc_squared - 2*apparent_wind_speed*speed_over_ground*(np.cos(angle))


    return np.sqrt(opposite_squared)
    

def find_true_angle(true_wind_speed,apparent_wind_angle, course_over_ground):
    true_wind_angle = 
    



#
def find_true_wind_direction(true_wind_angle):
    
def main():
    apparent_wind_speed = input("Apparent Wind Speed: ")
    apparent_wind_angle = input("Apparent Wind Angle: ")
    speed_over_ground = input("Speed Over Ground")
    course_over_ground = input("Course Over Ground")

    #angle between course over ground and apparent wind angle
    awa_cog_angle = angle_difference(apparent_wind_angle, course_over_ground)
    true_wind_speed = find_true_speed(awa_cog_angle, apparent_wind_speed, speed_over_ground)
    true_wind_angle = find_true_angle(true_wind_speed, apparent_wind_speed, course_over_ground)
    true_wind_direction = find_true_wind_direction(true_wind_angle)

    print "The True Wind Speed is: %s \n and the True Wind Direction is: %d" (true_wind_speed, true_wind_direction)

if __name__ == "__main__":
    main()
