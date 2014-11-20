#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include "udp.h"



#define server_ip "192.168.0.1"
#define port 9999
#define period 0.005

char buffer[255];
char buffer_pid[255];
pthread_mutex_t mutex_pid;
pthread_mutex_t signal;
int signal_flag = 0;
int new_measurment = 0;


void udp_listener(void* arg){
    struct udp_conn udpcon;
    udp_init_client(&udpcon, port, server_ip);
    char buffer[255];
    
    while(1){
        udp_receive(&udpcon, buffer, 255);
        if(buffer[0]=='G'){
            pthread_mutex_lock(&mutex_pid);
            buffer_pid = buffer;
            new_measurment = 1;
            pthread_mutex_unlock(&mutex_pid);
        }
        else if(buffer[0]=='S'){
            pthread_mutex_lock(&signal);
            signal_flag = 1;
            pthread_mutex_unlock(&signal);
        }      
    }
}

void send_get(void* arg){
    struct udp_conn udpcon;
    udp_init_client(&udpcon, port, server_ip);
    struct timespec next;
	clock_gettime(CLOCK_REALTIME, &next);		
	timespec_add_us(&next, (int)(period*1000000));

    while(1){
    	clock_nanosleep(&next);
	    timespec_add_us(&next, (int)(period*1000000));
	    udp_send(&udpcon, "GET", 4);
    }
}

void pid_thread(void* arg){

    struct udp_conn udpcon;
    udp_init_client(&udpcon, port, server_ip);

    

    double y, u;
    struct PID_parameters parameters= (struct PID_parameters){
		10,
		800,
		0,
		1,
		period,
	};
    
    char send_buffer[255];
    
    while(1){
        pthread_mutex_lock(&mutex_pid);
        if(new_measurment == 1){
            y = doubleFromReply(buffer_pid);
            pthread_mutex_unlock(&mutex_pid);
            
            u = PID_controller(parameters, y);
            
            setFromDouble(send_buffer, u);
            
            udp_send(&udpcon, send_buffer, 255);
        }else{pthread_mutex_unlock(&mutex_pid);}
    }
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

