#include <Servo.h>
#include <Stepper.h>

const int SPR= 200;

Servo myservo;
Stepper mystepper(SPR, 8, 9, 10, 11);
int pos= 0;
byte incomingByte;

void setup() {
  mystepper.setSpeed(60);
  myservo.attach(6);
  Serial.begin(9600);
}

void loop() {
  if(Serial.available()>0){
    incomingByte= Serial.read();
  
    if(incomingByte==',' && pos<=177){
      pos+=3;
      myservo.write(pos);
    }
    else if(incomingByte=='.' && pos>=3){
      pos-=3;
      myservo.write(pos);
    }
    else if(incomingByte=='h'){
      pos= 90;
      myservo.write(pos);
    }
    else if(incomingByte=='w'){
      mystepper.step(50);
    }
    else if(incomingByte=='s'){
      mystepper.step(-50);
    }
  }
}
