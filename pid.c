#include "pid.h"


float PID_controller(struct PID_parameters parameters, float measurement){
	static float integral;
	static float prev_error;
	float derivative;
	float error;

	error = parameters.reference - measurement;
	integral = integral + (error * parameters.period);
	derivative = error - prev_error;
	prev_error = error;
	return parameters.Kp * error + parameters.Ki*integral + parameters.Kd*derivative;
}

