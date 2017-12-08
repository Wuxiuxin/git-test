#include <stdio.h>
#include <string.h>

#include "include/rfid.h"

int main(void)
{
	car_iop head;
	
	head = car_io_init();
	
	car_io_data data;
/*
	strcpy(data.name, "wuxiuxin");
	strcpy(data.id, "0123456789");
	strcpy(data.time, "15:50:55");
	strcpy(data.mday, "15");
	strcpy(data.mon, "MAY");
	strcpy(data.wday, "MON");
	strcpy(data.year, "2017");
	
	
	car_io_insert(head, data);
*/	  

//	car_io_show(head);

	car_io_read_record(head);
	car_io_show(head);
	return 0;
}