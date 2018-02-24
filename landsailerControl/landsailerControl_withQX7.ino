/* 
 * rosserial subscriber to rudder heading
 * moves rudder to match heading
 */

#include <ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <Servo.h>
#include <Stepper.h>

byte manualMode_PIN = 3;
byte rudder_PIN = 5;
byte sail_PIN = 6;
double PWM_LO = 982;
double PWM_HI = 2001;
int manual_value;
int rudder_value; 
int rudder_pos; 
int sail_value;
int sail_pos;

const int SPR= 200;

Servo myservo;
Stepper mystepper(SPR, 8, 9, 10, 11);
int sheet= 0;

ros::NodeHandle  nh;

bool manualMode = false;

void rudderCb(const std_msgs::Int32 &rudderHeading){
  if (manualMode){
    myservo.write(rudder_pos); //changes rudder heading 
  }
  else{  
    myservo.write(rudderHeading.data); //changes rudder heading
  }
}

void sailCb(const std_msgs::Int32 &sailAngle){
  if (manualMode){
    mystepper.step(sail_pos-sheet);
    sheet = sheet + (sail_pos-sheet);
  }
  else{
    int temp= map(sailAngle.data, 0, 90, 0, 4500);
    mystepper.step(temp-sheet);
    sheet= sheet + (temp-sheet);
  }
  
}

ros::Subscriber<std_msgs::Int32> rudderSub("cmd_rudder_angle", &rudderCb );
ros::Subscriber<std_msgs::Int32> sailSub("cmd_sail_angle", &sailCb );

void setup()
{ 
  pinMode(manualMode_PIN, INPUT);
  pinMode(rudder_PIN, INPUT);
  pinMode(sail_PIN, INPUT);
  mystepper.setSpeed(60);
  myservo.attach(6);
  nh.initNode();
  nh.subscribe(rudderSub);
  nh.subscribe(sailSub);
}

void loop()
{  
  manual_value = pulseIn(manualMode_PIN, HIGH);
  if(manual_value>1500){
    manualMode = true;
  }
  else{
    manualMode = false;
  }
  rudder_value = pulseIn(rudder_PIN, HIGH);
  rudder_pos = (rudder_value - PWM_LO)/(PWM_HI - PWM_LO) * 180 - 90;
  sail_value = pulseIn(sail_PIN, HIGH);
  sail_pos = (sail_value - PWM_LO)/(PWM_HI - PWM_LO) * 90;
  
  nh.spinOnce(); //brings in info for subscriber
  delay(1);
}
