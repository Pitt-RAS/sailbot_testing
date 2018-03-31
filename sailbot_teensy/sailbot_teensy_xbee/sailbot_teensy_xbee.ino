// Combines sailbot_windsensors and sailbot_motors into one code
// (Basically the landsailer ardiuno code from the two ardiuno/teensys
// and combined into one for v1)
// Added code to pass info to land via xbee

#include <ros.h>
#include <tf/tf.h>
#include <sensor_msgs/Imu.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <nav_smgs/Odometry.h>
#include <geometry_msgs/NatSatFix.h>
#include <geometry_msgs/PointStamped.h>
#include "sensors/msg/TrueWind.h"
#include <tf/transform_broadcaster.h>
#include <PWMServo.h>
#include <Stepper.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <AS5045.h>
#include "Rate.h"
#include "HackyStepper.h"

void rudderCb(const std_msgs::Int32&);
void sailCb(const std_msgs::Int32&);
void trueWindCb(const sensors::msg::TrueWind&);
void goalCb(const geometry_msgs::PointStamped&);
void odomCb(const nav_msgs::Odometry&);
void gpsCb(const sensor_msgs::NatSatFix&);
void batteryCb(const std_msgs::Float32&);
void windISR();

// struct packet to send to land through xbee
const int32_t start_val = -1386103603;
struct serial_packet {
  int32_t serial_start;
  int32_t true_wind_speed;
  int32_t true_wind_dir;
  int32_t sail_angle; //from teensy code
  int32_t rudder_angle; //from teensy code
  int32_t state; //autonomous vs navigation, from teensy code
  float goal[2];
  float odom_heading;
  float odom_speed;
  float gps[2];
  float battery_volt;
} xbee_info;

ros::NodeHandle nh;

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

/*subscribers for xbee info*/
ros::Subscriber<sensors::msg::TrueWind> trueWindSub("/true_wind", &trueWindCb);
ros::Subscriber<geometry_msgs::PointStamped> goalPointSub("/goal", &goalCb);
ros::Subscriber<nav_msgs::Odometry> odomSub("/odometry/filtered", &odomCb);
ros::Subscriber<sensor_msgs::NatSatFix> gpsSub("/gps/fix", &gpsCb);
ross:Subscriber<std_msgs::Float32> batterySub("/battery", &batteryCb);

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
  xbee_info.rudder_angle = rudderHeading.data;
}

/*sail callback*/
void sailCb(const std_msgs::Int32 &sailAngle){
  int stepGoal = sailAngle.data;
  
  if(stepGoal>70) //70 degrees = max sail angle for landsailer
    stepGoal = 70;
   
  mystepper.setGoal(map(stepGoal, 0, 70, 0, 1500)); //maps angle to steps
}

/*true wind callback*/
void trueWindCb(const sensors::msg::TrueWind &trueWind) {
  xbee_info.true_wind_speed = trueWind.speed;
  xbee_info.true_wind_dir = trueWind.direction;
}

/*goal callback*/
void goalCb(const geometry_msgs::PointStamped &goal) {
  xbee_info.goal[0] = goal.point.x;
  xbee_info.goal[1] = goal.point.y;
}

/*odometry callback*/
void odomCb(const nav_msgs::Odometry &odom) {
  float x = odom.twist.twist.linear.x;
  float y = odom.twist.twist.linear.y;
  xbee_info.odom_speed = (x^2 + y^2)^0.5;

  float roll, pitch, yaw;
  tf::Quaternion quater;
  tf::quaternionMsgToTF(odom.pose.pose.orientation, quater);
  tf::Matrix3x3(quater).getRPY(roll, pitch, yaw);
  xbee_info.odom_heading = yaw;
}

/*gps callback*/
void gpsCb(const sensor_msgs::NatSatFix &gps) {
  xbee_info.gps[0] = gps.latitude;
  xbee_info.gps[1] = gps.longitude;
}

/*battery voltage callback*/
void batteryCb(const std_msgs::Float32 &batt) {
  xbee_info.battery_volt = batt.data;
}

void setup()
{ 
  Serial1.begin(9600);
  xbee_info.serial_start = start_val;
  
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
  nh.subscribe(trueWindSub);
  nh.subscribe(goalSub);
  nh.subscribe(odomSub);
  nh.subscribe(gpsSub);
  nh.subscribe(batterySub);
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
  xbee_info.sail_angle = mystepper.counter;

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

  Serial1.write((byte*)&xbee_info, sizeof(serial_packet));
  
  nh.spinOnce();
  delay(1);
}
