#include <ros.h>
#include <std_msgs/Float64.h>
#include <math.h>
#include <AS5045.h>

// CHANGE THESE AS APPROPRIATE
#define CSpin   3
#define CLKpin  2
#define DOpin   4

ros::NodeHandle nh;
std_msgs::Float64 float_msg;
ros::Publisher chatter("chatter", &float_msg);

float angle;

AS5045 enc (CLKpin, CSpin, DOpin) ;

// didn't program the OTP
void setup ()
{
  nh.initNode();
  nh.advertise(chatter);
  enc.begin();
  //Serial.begin (9600) ;   // NOTE BAUD RATE
 // if (!enc.begin ()){
   // Serial.println ("Error setting up AS5045") ;
    //while(1);
  //}
}

void loop ()
{
  delay(2000);
  int i = 0;
  const float pi = 3.1415927;
  float forget = 0.9; // 0 --> only newsest wind angle matters. 1 --> all past & present wind angles matters equally
  float sinweight = 0; 
  float cosweight = 0; 
  float sinval = 0;
  float cosval = 0; 
  float val;
  
  
  while(1) {
    val = enc.read () * .08789;
    //exponential weighting of sin component
    sinweight = forget*sinweight + 1;
    sinval = (1-1/sinweight)*sinval + (1/sinweight)*sin(val*pi/180);
    //sinval = sin(val*pi/180);

    //exponential weighting of cos component
    cosweight = forget*cosweight + 1;
    cosval = (1-1/cosweight)*cosval + (1/cosweight)*cos(val*pi/180);
    //cosval = cos(val*pi/180);
    //Serial.print ("Angle : ");
    //angle = atan2(sinval, cosval)*57.2958;
    //Serial.print ("\r");
    //delay(10);
    //i++;
    float_msg.data = atan2f(sinval, cosval)*57.2958;
    chatter.publish( &float_msg );
    nh.spinOnce();
    delay(100);
    }
  //Serial.end();
}
