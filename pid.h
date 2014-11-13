
#ifndef PID_H_
#define PID_H_

struct PID_parameters{
	float Kp;
	float Ki;
	float Kd;
	float reference;
	float period;
	float error;
};



float PID_controller(struct PID_parameters parameters, float measurement);



#endif /* PID_H_ */
