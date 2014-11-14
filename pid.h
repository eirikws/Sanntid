
#ifndef PID_H_
#define PID_H_

struct PID_parameters{
	double Kp;
	double Ki;
	double Kd;
	double reference;
	double period;
};



double PID_controller(struct PID_parameters parameters, double measurement);



#endif /* PID_H_ */
