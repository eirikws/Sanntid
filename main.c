
#include <stdio.h>
#include "udp.h"
#include "pid.h"

int main(void){


	udp_init_client(udp_conn, port, ip);



	struct PID_parameters parameters= (struct PID_parameters){
		1,
		1,
		0,
		1,
		0.01,
		0
	};




	int a=PID_controller(parameters, 2);

	printf("pid: %d\n", a);

	return 0;
}
