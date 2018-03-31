#include <Arduino.h>
#include <Stepper.h>
#include "HackyStepper.h"

bool running = false;
int counter = 0;
int goal = 0;

HackyStepper::HackyStepper(int number_of_steps, int motor_pin_1, int motor_pin_2, int motor_pin_3, int motor_pin_4)
: Stepper(number_of_steps, motor_pin_1, motor_pin_2, motor_pin_3, motor_pin_4) {
}

void HackyStepper::setGoal(int goal) {
  this->goal = goal;
}

int HackyStepper::getPosition() {
  return counter;
}

int HackyStepper::getGoal() {
  return goal;
}

void HackyStepper::update() {
  running = counter != goal;
  direction = counter > goal;
  
  if ( running ) {
    unsigned long now = micros();
    if (now - this->last_step_time >= this->step_delay)
    {
      // get the timeStamp of when you stepped:
      this->last_step_time = now;
      // increment or decrement the step number,
      // depending on direction:
      if (this->direction == 1)
      {
        this->step_number++;
        counter--;
        if (this->step_number == this->number_of_steps) {
          this->step_number = 0;
        }
      }
      else
      {
        if (this->step_number == 0) {
          this->step_number = this->number_of_steps;
        }
        this->step_number--;
        counter++;
      }
      // step the motor to step number 0, 1, ..., {3 or 10}
      if (this->pin_count == 5)
        stepMotor(this->step_number % 10);
      else
        stepMotor(this->step_number % 4);
    }
  }
}
