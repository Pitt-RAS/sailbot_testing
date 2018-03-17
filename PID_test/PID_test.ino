#include "pid.h"
#include <Servo.h>

#define pwmOut_pin A1
#define Kp 1.65
#define Ki 0
#define Kd 0.75

Servo myservo;
PID pid = PID(Kp, Kd, Ki);

String inputString = "";
bool outputting = true;

void setup() {
  // put your setup code here, to run once:
  pinMode(pwmOut_pin, OUTPUT);
  myservo.attach(6);
  Serial.begin(9600);

  pid.setSetpoint(140);

  Serial.print("Started");
}

void loop() {
  // put your main code here, to run repeatedly:
  //delay(10);
  int potValue = analogRead(A0); // Read potentiometer value
  int motorAngle = map(potValue, 0, 1023, 0 , 280); // Map the potentiometer value from 0 to 280
  double pidOut = pid.calculate(motorAngle);
  double spd = 90;
  if(pidOut > 250){
    spd = 180;
  } else if(pidOut < -250){
    spd = 0;
  } else {
    spd = map(pidOut, -250, 250, 0, 180); 
  }
  
  /*
  //This way should work but it doesn't (T^T)
  double spd = pidOut/4 + 90;
  if(spd > 180){
    spd = 180;
  }
  if(spd < 0){
    spd = 0;
  }
  */
  myservo.write(spd);
}




