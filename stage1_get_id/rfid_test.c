#include <stdio.h>
#include <fcntl.h> 
#include <unistd.h>
#include <termios.h> 
#include <sys/types.h>
#include <sys/select.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>

#include "rfid.h"


#define SERIAL_PATH1	"/dev/s3c2410_serial1"
#define SERIAL_PATH3	"/dev/s3c2410_serial3"




extern struct timeval timeout;
//extern volatile unsigned int cardid;

char cardid_in[64];
char cardid_out[64];

void *car_in(void *arg)
{
	int ret;
	int fd_in;
	unsigned long cardid;
	
	fd_in = open(SERIAL_PATH1, O_RDWR);
	if(fd_in < 0)
	{
		printf("open s3c2410_serial1 failed!\n");
		return NULL;
	}
	
	
	if(init_tty(fd_in) == -1)//初始化串口
	{
		printf("init_tty in failed!\n");
	}
	
	timeout.tv_sec = 1;//设置超时时间为1秒
	timeout.tv_usec = 0;
		
	while(1)
	{
		
		
		if(PiccRequest(fd_in))//请求
		{
			printf("PiccRequest car in failed!\n");
			usleep(500000);
			continue;
		}
		
		tcflush(fd_in, TCIOFLUSH);//清空进车串口IO缓冲区
		usleep(300000);
		
		if((cardid = PiccAnticoll(fd_in)) == 0)
		{
			printf("PiccAnticoll car in failed!\n");
			usleep(300000);
			continue;
		}
		else
		{
			printf("Begin to change to string!-->car in\n");
			sprintf(cardid_in, "%ul", cardid); 
		}
		tcflush(fd_in, TCIOFLUSH  );
		
		printf("in card ID = %s\n", cardid_in);
		
		
	}
	
	close(fd_in);
	
	pthread_exit(NULL);
	
}

void *car_out(void *arg)
{
	int ret;
	int fd_out;
	unsigned long cardid;
	
	fd_out = open(SERIAL_PATH3, O_RDWR);
	if(fd_out < 0)
	{
		printf("open s3c2410_serial3 failed!\n");
		return NULL;
	}
	
	if(init_tty(fd_out) == -1)//初始化串口
	{
		printf("init_tty out failed!\n");
	}
	
	timeout.tv_sec = 1;//设置超时时间为1秒
	timeout.tv_usec = 0;
	
	while(1)
	{
		
		
		if(PiccRequest(fd_out))//请求
		{
			printf("PiccRequest car out failed!\n");
			usleep(500000);
			continue;
		}
		
		tcflush(fd_out, TCIOFLUSH);//清空进车串口IO缓冲区
		usleep(300000);
			
		if((cardid = PiccAnticoll(fd_out)) == 0)
		{
			printf("PiccAnticoll car out failed!\n");
			usleep(300000);
			continue;
		}
		else
		{
			printf("Begin to change to string!-->car out\n");
			sprintf(cardid_out, "%ul", cardid);
		}
		tcflush(fd_out, TCIOFLUSH);
		
		printf("out card ID = %s\n", cardid_out);
		
		
	}
	
	close(fd_out);
	pthread_exit(NULL);
	
}



int main()
{
	pthread_t in;
	pthread_t out;
	
	
	pthread_create(&in, NULL, &car_in, NULL);
	pthread_create(&out, NULL, &car_out, NULL);
	
	
	
	
	
	pthread_join(in, NULL);
	pthread_join(out, NULL);

	
	return 0;
}