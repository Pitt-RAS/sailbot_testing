#include "pid.h"
#include <Servo.h>

#define pwmOut_pin A1
#define Kp 0.5
#define Ki 0
#define Kd 0

Servo myservo;
PID pid = PID(Kp, Kd, Ki); 

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
  delay(10);
  int potValue = analogRead(A0); // Read potentiometer value
  int motorAngle = map(potValue, 0, 1023, 0 , 280); // Map the potentiometer value from 0 to 280
  
  Serial.println("");
  Serial.println("Pot Value:");
  Serial.println(motorAngle, DEC);

  
  double pidOut = pid.calculate(motorAngle);
  int spd = map(pidOut, -100, 100, 0, 180); // Map the potentiometer value from 0 to 280
  
  Serial.println("");
  Serial.println("PID Output");
  Serial.println(spd, DEC);

  myservo.write(spd);
}





