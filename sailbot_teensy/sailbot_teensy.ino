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
#include "imuRate.h"

class HackyStepper : public Stepper {
  bool running = false;
  int counter = 0;
  int goal = 0;
  public:
  HackyStepper(int number_of_steps, int motor_pin_1, int motor_pin_2, int motor_pin_3, int motor_pin_4)
  : Stepper(number_of_steps, motor_pin_1, motor_pin_2, motor_pin_3, motor_pin_4) {
  }

    void setGoal(int goal) {
      this->goal = goal;
    }

    int getPosition() {
      return counter;
    }

    int getGoal() {
      return goal;
    }


    void update() {
      running = counter != goal;
      direction = counter > goal;
      
      if ( running ) {
        unsigned long now = micros();
        if (now - this->last_step_time >= this->step_delay)
        {
          // get the timeStamp of when you stepped:
          this->last_step_time = now;
          // increment or decrement the step number,
          // depending on direction:
          if (this->direction == 1)
          {
            this->step_number++;
            counter--;
            if (this->step_number == this->number_of_steps) {
              this->step_number = 0;
            }
          }
          else
          {
            if (this->step_number == 0) {
              this->step_number = this->number_of_steps;
            }
            this->step_number--;
            counter++;
          }
          // step the motor to step number 0, 1, ..., {3 or 10}
          if (this->pin_count == 5)
            stepMotor(this->step_number % 10);
          else
            stepMotor(this->step_number % 4);
        }
      }
    }
};
 
unsigned long windSensorLastTick = millis();
unsigned long windSensorDt = 0;
bool windSensorDtUpdated = false;

const int SPR= 200;

Adafruit_BNO055 bno = Adafruit_BNO055(55);
AS5045 angleSensor(A2, A3, A4) ;
PWMServo myservo;
HackyStepper mystepper(SPR, 14, 15, 16, 17);
imuRate rate(100);

void windISR() {
  if ( windSensorDtUpdated )
    return;
  windSensorDt = millis() - windSensorLastTick;
  windSensorLastTick = millis();
  windSensorDtUpdated = true;
}

void rudderCb(const std_msgs::Int32 &rudderHeading){
  myservo.write(rudderHeading.data); //changes rudder heading
}

void sailCb(const std_msgs::Int32 &sailAngle){
  int stepGoal = sailAngle.data;
  
  if(stepGoal>70)
    stepGoal = 70;
   
  mystepper.setGoal(map(stepGoal, 0, 70, 0, 1500));
}

ros::NodeHandle  nh;

std_msgs::Int32 windSensorTick;
ros::Publisher windSensorTickPublisher("wind_sensor_tick", &windSensorTick);

std_msgs::Int32 relativeWindDirection;
ros::Publisher relativeWindDirectionPublisher("/relative_wind_direction/raw", &relativeWindDirection);

ros::Subscriber<std_msgs::Int32> rudderSub("cmd_rudder_angle", &rudderCb );
ros::Subscriber<std_msgs::Int32> sailSub("cmd_sail_angle", &sailCb );
sensor_msgs::Imu imu_msg;
ros::Publisher imuPub("/imu", &imu_msg);

void setup()
{ 
  angleSensor.begin();
  mystepper.setSpeed(60);
  myservo.attach(6);
  nh.initNode();
  nh.subscribe(rudderSub);
  nh.subscribe(sailSub);
  nh.advertise(imuPub);
  nh.advertise(windSensorTickPublisher);
  nh.advertise(relativeWindDirectionPublisher);
  
  pinMode(13, OUTPUT);
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), windISR, RISING);
  
  //I2C connection is created
  Wire.begin();
  
  bno.begin(Adafruit_BNO055::OPERATION_MODE_IMUPLUS);
  
  imu_msg.header.frame_id = "base_link";
}

void loop()
{ 
  digitalWrite(13, HIGH);

  mystepper.update();

  if(rate.needsRun())
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
  
  digitalWrite(13, LOW);
  
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

