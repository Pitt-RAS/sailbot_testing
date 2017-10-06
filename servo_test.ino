#include <Servo.h>
#include <Stepper.h>

const int SPR= 200;

Servo myservo;
Stepper mystepper(SPR, 8, 9, 10, 11);
int pos= 0;
byte incomingByte;

void setup() {
  Serial.begin(9600);
  myservo.attach(6);
}

void loop() {
  if(Serial.available()>0){
    incomingByte= Serial.read();
  
    if(incomingByte=='.' && pos<=178){
      pos+=2;
      myservo.write(pos);
    }
    else if(incomingByte==',' && pos>=2){
      pos-=2;
      myservo.write(pos);
    }
    else if(incomingByte=='w'){
      mystepper.step(2);
    }
    else if(incomingByte=='s'){
      mystepper.step(-2);
    }
  }
}
