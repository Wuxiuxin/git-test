#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "rfid.h"

extern car_io_insert(car_iop head, car_io_data data);
extern car_io_show(car_iop head);
extern car_io_init(void);
int main()
{
	char buf[sizeof(car_io_data)];
	car_io_data data;
	
	car_iop head = car_io_init();

	FILE *fp = fopen("car_io.txt", "r");
	
	if(fp == NULL)
	{
		printf("Open car_io.txt failed!\n");
	}
		
	memset(buf, 0, sizeof(car_io_data));
	fread(buf, sizeof(car_io_data), 1, fp);
	
	sscanf(buf, "%s%s%s%d%d%d", 
		data.name,
		data.id,
		data.time,
		&data.day,
		&data.year,
		&data.month
	);	
	
	car_io_insert(head, data);
	car_io_show(head);
/*	printf("%-8s\t%s\t%s\t%d\t%d\t%d\t\n",
		data.name,
		data.id,
		data.time,
		data.day,
		data.year,
		data.month
		);*/
	fclose(fp);
	return 0;

}
