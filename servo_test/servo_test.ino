#include <Servo.h>
#include <Stepper.h>

const int SPR= 200;

Servo myservo;
Stepper mystepper(SPR, 8, 9, 10, 11);
int rudder= 0;
int sheets= 0;
byte incomingByte;

void setup() {
  mystepper.setSpeed(60);
  myservo.attach(6);
  Serial.begin(9600);
}

void loop() {
  if(Serial.available()>0){
    incomingByte= Serial.read();
  
    if(incomingByte==',' && rudder<=177){
      rudder+=3;
      myservo.write(rudder);
    }
    else if(incomingByte=='.' && rudder>=3){
      rudder-=3;
      myservo.write(rudder);
    }
    else if(incomingByte=='h'){
      rudder= 90;
      myservo.write(rudder);
    }
    else if(incomingByte=='w' && sheets>=10){
      sheets-=10;
      mystepper.step(50);
    }
    else if(incomingByte=='s' && sheets<=290){
      sheets+=10;
      mystepper.step(-50);
    }
  }
}
