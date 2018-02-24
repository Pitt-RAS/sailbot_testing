// February 24, 2018 by Nate Riek and Grace Chu
// This code takes input from the QX7 controller to control
// the sail and rudder positions in degress



byte rudderPIN = 9; //comes from pin 
byte sailPIN = 10; //comes from pin1 on r9
double PWM_LO = 982;
double PWM_HI = 2001;
int rudder_value;  //input from RC receiver
int rudder_pos;  //mapped rudder position (-90 to 90 degrees)
int sail_value;
int sail_pos;

void setup() {
  pinMode(rudderPIN, INPUT);
  pinMode(sailPIN, INPUT);
  Serial.begin(9600);
}

// -90 to 90 (rudder position)
// 982 to 2001 (rudder pwm input)
// 0 to 90 (sail position)
// 982 to 2001 (sail pwm input)

void loop() {
  rudder_value = pulseIn(rudderPIN, HIGH);
  rudder_pos = (rudder_value - PWM_LO)/(PWM_HI - PWM_LO) * 180 - 90;
  sail_value = pulseIn(sailPIN, HIGH);
  sail_pos = (sail_value - PWM_LO)/(PWM_HI - PWM_LO) * 90;
  Serial.println(sail_pos);
  //Serial.println(sail_pos);
}
