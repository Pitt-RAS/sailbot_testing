
#include <Servo.h>
#include <Stepper.h>

Servo myservo;    
Stepper myStepper(SPR,8,9,10,11);
const int SPR = 200;
int pos = 0; 

void setup() {
  Serial.begin(9600);
  myservo.attach(6);  
}

void loop() {
  char c = Serial.read();
  if (c == 'l' && pos!=0){
    pos-=5;
  }
  if (c == 'r' && pos!= 180){
      pos+=5;
  }
  if (c == 'a'){
    myStepper.step(-1);
  }
  if(c == 's'){
    myStepper.step(1)
  }
    myservo.write(pos);    
  
}

