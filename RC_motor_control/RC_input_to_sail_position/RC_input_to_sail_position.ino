// February 10, 2018 by Grace Chu
// This code takes in the servo input from the RC receiver and 
// converts the PWM value into a sail position in degrees.

byte PWM_PIN = 9;
double PWM_LO = 982;
double PWM_HI = 2001;
int pwm_value;  //input from RC receiver
int pos;  //mapped sail position (0 to 90 degrees)

void setup() {
  pinMode(PWM_PIN, INPUT);
  Serial.begin(9600);
}

// 0 to 90    pos(ition)
// 982 to 2001    pwm input

void loop() {
  pwm_value = pulseIn(PWM_PIN, HIGH);
  pos = (pwm_value - PWM_LO)/(PWM_HI - PWM_LO) * 90;
  Serial.println(pos);
}
