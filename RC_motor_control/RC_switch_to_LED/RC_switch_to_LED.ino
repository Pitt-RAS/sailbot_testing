// February 10, 2018 by Grace Chu
// This code takes in the PWM input from the RC receiver and turns
// an LED on and off (from a flipped switch on the transmitter

byte PWM_PIN = 9;
int LED_PIN = 7;
int pwm_value;

void setup() {
  pinMode(PWM_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  pwm_value = pulseIn(PWM_PIN, HIGH);
  if(pwm_value>1500) {
    digitalWrite(LED_PIN, HIGH);
  }
  else {
    digitalWrite(LED_PIN, LOW);
  }
  Serial.println(pwm_value);
}
