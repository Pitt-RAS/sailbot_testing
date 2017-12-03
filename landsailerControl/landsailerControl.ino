/* 
 * rosserial subscriber to rudder heading
 * moves rudder to match heading
 */

#include <ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <Servo.h>
#include <Stepper.h>

const int SPR= 200;

Servo myservo;
Stepper mystepper(SPR, 8, 9, 10, 11);
int sheet= 0;

ros::NodeHandle  nh;

void rudderCb(const std_msgs::Int32 &rudderHeading){
  myservo.write(rudderHeading.data); //changes rudder heading
}

void sailCb(const std_msgs::Int32 &sailAngle){
  int temp= map(sailAngle.data, 0, 90, 0, 4500);
  mystepper.step(temp-sheet);
  sheet= sheet + (temp-sheet);
}

ros::Subscriber<std_msgs::Int32> rudderSub("cmd_rudder_angle", &rudderCb );
ros::Subscriber<std_msgs::Int32> sailSub("cmd_sail_angle", &sailCb );

void setup()
{ 
  mystepper.setSpeed(60);
  myservo.attach(6);
  nh.initNode();
  nh.subscribe(rudderSub);
  nh.subscribe(sailSub);
}

void loop()
{  
  nh.spinOnce(); //brings in info for subscriber
  delay(1);
}

