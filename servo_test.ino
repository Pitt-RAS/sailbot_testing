#include <Servo.h>

Servo myservo;
int pos= 0;
byte incomingByte;

void setup() {
  Serial.begin(9600);
  myservo.attach(9);
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
  }
  Serial.print(pos);
}
