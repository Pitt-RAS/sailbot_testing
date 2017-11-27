#include <ros.h>
#include <geometry_msgs/Vector3Stamped.h>

//This is a spark fun library with all of the variables defined
#include <MPU9250.h>

MPU9250 mpuIMU;


//Definign the rosserial node
ros::NodeHandle imuNodeHandler;

geometry_msgs::Vector3Stamped accelerometer_msg;
ros::Publisher accelerometer("IMUAccelerometer", &accelerometer_msg);


geometry_msgs::Vector3Stamped gyrometer_msg;
ros::Publisher gyrometer("IMUGyrometer", &gyrometer_msg);


geometry_msgs::Vector3Stamped magnetometer_msg;
ros::Publisher magnetometer("IMUMagnetometer", &magnetometer_msg);


void setup() {
  //I2C connection is created
  Wire.begin();
  
  Serial.begin(9600);
  
  //Has to be 0x73 because this is an MPU9255.
  if(mpuIMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250) == 0x73)
  {
    //Sets up the device
    mpuIMU.calibrateMPU9250(mpuIMU.gyroBias, mpuIMU.accelBias);
    mpuIMU.initMPU9250();
    mpuIMU.initAK8963(mpuIMU.magCalibration);
  }

  //Dose the rosserial things
  imuNodeHandler.initNode();
  imuNodeHandler.advertise(accelerometer);
  imuNodeHandler.advertise(gyrometer);
  imuNodeHandler.advertise(magnetometer);
  
}

void loop() {

  //Every time the data is ready to be read the code gets exacuted
  if (mpuIMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
  {  

    //Calculates the accelerometer data
    mpuIMU.readAccelData(mpuIMU.accelCount);
    mpuIMU.getAres();
    mpuIMU.ax = (float)mpuIMU.accelCount[0]*mpuIMU.aRes - mpuIMU.accelBias[0];
    mpuIMU.ay = (float)mpuIMU.accelCount[1]*mpuIMU.aRes - mpuIMU.accelBias[1];
    mpuIMU.az = (float)mpuIMU.accelCount[2]*mpuIMU.aRes - mpuIMU.accelBias[2];

    //Updates the rosserial message
    accelerometer_msg.vector.x = mpuIMU.ax;
    accelerometer_msg.vector.y = mpuIMU.ay;
    accelerometer_msg.vector.z = mpuIMU.az;


    //Calculates the gyrometer data
    mpuIMU.readGyroData(mpuIMU.gyroCount);
    mpuIMU.getGres();
    mpuIMU.gx = (float)mpuIMU.gyroCount[0]*mpuIMU.gRes;
    mpuIMU.gy = (float)mpuIMU.gyroCount[1]*mpuIMU.gRes;
    mpuIMU.gz = (float)mpuIMU.gyroCount[2]*mpuIMU.gRes;

    //Updates the rosserial message
    gyrometer_msg.vector.x = mpuIMU.gx;
    gyrometer_msg.vector.y = mpuIMU.gy;
    gyrometer_msg.vector.z = mpuIMU.gz;


    //Calculates the magnetometer data
    mpuIMU.readMagData(mpuIMU.magCount);
    mpuIMU.getMres();
    mpuIMU.mx = (float)mpuIMU.magCount[0]*mpuIMU.mRes*mpuIMU.magCalibration[0];
    mpuIMU.my = (float)mpuIMU.magCount[1]*mpuIMU.mRes*mpuIMU.magCalibration[1];
    mpuIMU.mz = (float)mpuIMU.magCount[2]*mpuIMU.mRes*mpuIMU.magCalibration[2];
    
    //Updates the rosserial message
    magnetometer_msg.vector.x = mpuIMU.mx;
    magnetometer_msg.vector.y = mpuIMU.my;
    magnetometer_msg.vector.z = mpuIMU.mz;
    
  }
}
