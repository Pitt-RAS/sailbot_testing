#ifndef HACKYSTEPPER_H
#define HACKYSTEPPER_H

class HackyStepper : public Stepper {
public:
  HackyStepper(int, int, int, int, int);
  void setGoal(int);
  int getPosition();
  int getGoal();
  void update();
  bool running;
  int counter;
  int goal;
};

#endif
