// Combines sailbot_windsensors and sailbot_motors into one code
// (Basically the landsailer ardiuno code from the two ardiuno/teensys
// and combined into one for v1)

#include <ros.h>
#include <tf/tf.h>
#include <sensor_msgs/Imu.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <PWMServo.h>
#include <Stepper.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <AS5045.h>
#include "Rate.h"
#include "HackyStepper.h"

void rudderCb(const std_msgs::Int32&);
void sailCb(const std_msgs::Int32&);
void windISR();

ros::NodeHandle  nh;

/*wind speed publisher*/
std_msgs::Int32 windSensorTick;
ros::Publisher windSensorTickPublisher("wind_sensor_tick", &windSensorTick);

/*wind direction publiser*/
std_msgs::Int32 relativeWindDirection;
ros::Publisher relativeWindDirectionPublisher("/relative_wind_direction/raw", &relativeWindDirection);

/*imu publisher*/
sensor_msgs::Imu imu_msg;
ros::Publisher imuPub("/imu", &imu_msg);

/*rudder and sail subscribers*/
ros::Subscriber<std_msgs::Int32> rudderSub("cmd_rudder_angle", &rudderCb );
ros::Subscriber<std_msgs::Int32> sailSub("cmd_sail_angle", &sailCb );

/*wind speed global vars*/
unsigned long windSensorLastTick = millis();
unsigned long windSensorDt = 0;
bool windSensorDtUpdated = false;

/*stepper motor*/
const int SPR= 200;
HackyStepper mystepper(SPR, 6, 7, 8, 9);

/*IMU*/
Adafruit_BNO055 bno = Adafruit_BNO055(55);
Rate imuRate(100);

/*wind direction sensor*/
AS5045 angleSensor(24, 25, 26) ;

PWMServo myservo;

/*wind speed interrupt handler*/
void windISR() {
  if ( windSensorDtUpdated )
    return;
  windSensorDt = millis() - windSensorLastTick;
  windSensorLastTick = millis();
  windSensorDtUpdated = true;
}

/*rudder callback*/
void rudderCb(const std_msgs::Int32 &rudderHeading){
  myservo.write(rudderHeading.data); //changes rudder heading
}

/*sail callback*/
void sailCb(const std_msgs::Int32 &sailAngle){
  int stepGoal = sailAngle.data;
  
  if(stepGoal>70) //70 degrees = max sail angle for landsailer
    stepGoal = 70;
   
  mystepper.setGoal(map(stepGoal, 0, 70, 0, 1500)); //maps angle to steps
}

void setup()
{ 
  angleSensor.begin();
  
  mystepper.setSpeed(60);
  
  myservo.attach(5);

  bno.begin(Adafruit_BNO055::OPERATION_MODE_IMUPLUS);
  Wire.begin(); //I2C connection is created (for IMU)

  pinMode(13, OUTPUT);
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), windISR, RISING);
  
  nh.initNode();
  nh.subscribe(rudderSub);
  nh.subscribe(sailSub);
  nh.advertise(imuPub);
  nh.advertise(windSensorTickPublisher);
  nh.advertise(relativeWindDirectionPublisher);
  
  imu_msg.header.frame_id = "boat";
}

void loop()
{ 
  int heartbeat = digitalRead(13); //toggles heartbeat LED
  if(heartbeat == LOW)
    digitalWrite(13, HIGH);
  else
    digitalWrite(13, LOW);
    
  mystepper.update();

  if(imuRate.needsRun()) //sends IMU info at rate set by imuRate
  {
    imu::Quaternion imuQuat = bno.getQuat();
    imu::Vector<3> linearAccel = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
    imu::Vector<3> angularVel = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    
    imu_msg.header.stamp = nh.now();
    
    imu_msg.orientation.x = imuQuat.x();
    imu_msg.orientation.y = imuQuat.y();
    imu_msg.orientation.z = imuQuat.z();
    imu_msg.orientation.w = imuQuat.w();
    
    imu_msg.angular_velocity.x = angularVel.x();
    imu_msg.angular_velocity.y = angularVel.y();
    imu_msg.angular_velocity.z = angularVel.z();
    
    imu_msg.linear_acceleration.x = linearAccel.x();
    imu_msg.linear_acceleration.y = linearAccel.y();
    imu_msg.linear_acceleration.z = linearAccel.z();
    
    imuPub.publish(&imu_msg);

    //char debug[128];
    //sprintf(debug, "at %d, target is %d", mystepper.getPosition(), mystepper.getGoal());
    //nh.logwarn(debug);
  }
  
  if ( windSensorDtUpdated ) {
    windSensorTick.data = windSensorDt;
    windSensorTickPublisher.publish(&windSensorTick);
    windSensorDtUpdated = false;
  }

  relativeWindDirection.data = angleSensor.read();
  relativeWindDirectionPublisher.publish(&relativeWindDirection);
  
  nh.spinOnce();
  delay(1);
}

