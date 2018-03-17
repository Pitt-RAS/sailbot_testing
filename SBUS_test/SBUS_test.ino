#include <SBUS.h>

SBUS r9(Serial1);

uint16_t channels[16];
uint8_t failSafe;
uint16_t lostFrames = 0;

void setup() {
  r9.begin();
  Serial.begin(9600);
}

void loop() {
  if(r9.read(&channels[0], &failSafe, &lostFrames))
  {
    Serial.println(channels[1]);  
//    Serial.print(channels[1] + " "); 
//    Serial.print(channels[2] + " "); 
//    Serial.print(channels[3] + " "); 
//    Serial.print(channels[4] + " "); 
//    Serial.print(channels[5] + " "); 
//    Serial.print(channels[6] + " "); 
//    Serial.print(channels[7] + " "); 
//    Serial.print(channels[8] + " "); 
//    Serial.print(channels[9] + " "); 
//    Serial.print(channels[10] + " "); 
//    Serial.print(channels[11] + " "); 
//    Serial.print(channels[12] + " "); 
//    Serial.print(channels[13] + " "); 
//    Serial.print(channels[14] + " "); 
//    Serial.println(channels[15]);  
  }
}
