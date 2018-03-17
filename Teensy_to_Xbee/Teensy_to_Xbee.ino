//Grace Chu, Feb 28, 2018
//This code allows for communication between the Teensy 3.5 (with
// Xbee on RX1 and TX1) and the other Xbee.

void setup() {
  Serial.begin(9600); //USB serial
  Serial1.begin(9600); //Serial connected to Xbee (RX1 and TX1)
  delay(100);
  Serial1.write('B'); //Sets Xbee to Bypass mode to allow for radio
                      //communication
  delay(100);
}

void loop() {
  //Serial1.write("Hello world");
  if(Serial.available()) {
    Serial1.write(Serial.read());
  }
  if(Serial1.available()) {
    Serial.write(Serial1.read());
  }
}

