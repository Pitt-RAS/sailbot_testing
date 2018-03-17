#ifndef PID_H
#define PID_H

class PID {

	double kP;
	double kI;
	double kD;
	double setpoint;
	double integral;
	double lastTime;
	double lastError;
 
public:

	PID(double kP, double kI, double kD);
	void setSetpoint(double setpoint);
	double calculate(double actual);
	double now();
	
};

#endif