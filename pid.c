#include "pid.h"
#include <stdio.h>


double PID_controller(struct PID_parameters parameters, double measurement){
	static double integral;
	static double prev_error;
	double derivative;
	double error;

	error = parameters.reference - measurement;
	integral = integral + (error * parameters.period);
	derivative = (error - prev_error)/parameters.period;
	prev_error = error;
	
	printf("error, prop, integral: %f,%f, %f \n",error,parameters.Kp * error, integral);
	//printf("proportional, integral, derivative: %f, %f, %f",parameters.Kp * error,parameters.Ki*integral, parameters.Kd*derivative  );
	//printf("pid: %f\n", parameters.Kp * error + parameters.Ki*integral + parameters.Kd*derivative);
	return parameters.Kp * error + parameters.Ki*integral + parameters.Kd*derivative;
}

