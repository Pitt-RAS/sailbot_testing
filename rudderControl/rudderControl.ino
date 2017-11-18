/* 
 * rosserial subscriber to rudder heading
 * moves rudder to match heading
 */

#include <ros.h>
#include <Servo.h>
#include <std_msgs/Int32.h>

Servo myservo;
ros::NodeHandle  nh;

void messageCb( const std_msgs::Int32& rudderHeading){
  myservo.write(int(rudderHeading)); //changes rudder heading
}

ros::Subscriber<std_msgs::Int32> sub("new_rudder_heading", &messageCb );

void setup()
{ 
  myservo.attach(6);
  nh.initNode();
  nh.subscribe(sub);
}

void loop()
{  
  nh.spinOnce(); //brings in info for subscriber
  delay(1);
}

