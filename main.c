
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "udp.h"
#include "pid.h"


double doubleFromReply(char* reply);
void setFromDouble(char *buffer, double u);

#define client_ip "192.168.0.10"
#define server_ip "192.168.0.1"
#define port 9999
#define period 0.005
int main(void){
    struct udp_conn udpcon;
    
    
    
	udp_init_client(&udpcon, port, server_ip);
    
    char buffer[255];
           
	struct PID_parameters parameters= (struct PID_parameters){
		10,
		800,
		0,
		1,
		period,
	};
	double y, u;
	int i=0;
	struct timespec next;
	clock_gettime(CLOCK_REALTIME, &next);		
	timespec_add_us(&next, (int)(period*1000000));
	udp_send(&udpcon, "START", 6);
	while(1){
	    udp_send(&udpcon, "GET", 4);
	    
	    while(1){
	        udp_receive(&udpcon, buffer, 255);
	        if (buffer[0]=='G'){
	            break;
	        }
	    }
	    
	    y = doubleFromReply(buffer);
	    u = PID_controller(parameters, y);
	    
	    setFromDouble(buffer,u);
	    udp_send(&udpcon, buffer, 255);
	    clock_nanosleep(&next);
	    timespec_add_us(&next, (int)(period*1000000));
	    if (i++*period >= 0.5){
	        break;
	    }
	}
	
	udp_send(&udpcon, "STOP",5);
	printf("stop\n");
	return 0;
}

void setFromDouble(char *buffer, double u){
    buffer[0]='S';
    buffer[1]='E';
    buffer[2]='T';
    buffer[3]=':';
    char dummy[255];
    sprintf(dummy, "%f", u);
    char* ptr = dummy;
    int i=4;
    while(*ptr !='\0'){
        buffer[i++]=*ptr;
        ptr++;
    }
    buffer[i]='\0';
    return;
}

double doubleFromReply(char* reply){
    char* ptr;
	ptr=&reply[8];
	char dummy[255];
	int i=0;
	while(*ptr != '\0'){
	    dummy[i++]=*ptr;
	    ptr++;
	}
	dummy[i]='\0';
	return atof(dummy);
}


