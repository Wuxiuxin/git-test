#include <stdio.h>
#include <time.h>
#include <string.h>

#include "include/rfid.h"

struct car_get_time car_get_time()
{
	//	char array[26] = {0};
	char date[25];	
	unsigned int hour;
	
	struct car_get_time get_time;

	time_t timep;
//	time(&timep);
	
		time(&timep);
		sprintf(date,"%s",ctime(&timep));
		printf("date=%s\n",date);

//		hour = (date[11]-48)*10+(date[12]-48);
		memset(get_time.tm_sec, 0, sizeof(get_time.tm_sec));
		memset(get_time.tm_min, 0, sizeof(get_time.tm_min));
		memset(get_time.tm_hour, 0, sizeof(get_time.tm_hour));
		memset(get_time.tm_time, 0, sizeof(get_time.tm_time));
		memset(get_time.tm_mday, 0, sizeof(get_time.tm_mday));
		memset(get_time.tm_mon, 0, sizeof(get_time.tm_mon));
		memset(get_time.tm_wday, 0, sizeof(get_time.tm_wday));
		memset(get_time.tm_year, 0, sizeof(get_time.tm_year));
		memset(get_time.tm_date, 0, sizeof(get_time.tm_date));
		
		memcpy(get_time.tm_sec, date+17, 2);
		memcpy(get_time.tm_min, date+14, 2);
		memcpy(get_time.tm_hour, date+11, 2);
		memcpy(get_time.tm_time, date+11, 9);
		memcpy(get_time.tm_mday, date+8, 2);
		memcpy(get_time.tm_mon, date+4, 3);
		memcpy(get_time.tm_wday, date+0, 3);
		memcpy(get_time.tm_year, date+20, 4);
		memcpy(get_time.tm_date, date+0, 25);
		
		
		return get_time;	
}
/*
int main(void)
{
	struct car_get_time get_time;
	
	get_time = car_get_time();

		
	printf("tm_hour=%s ", get_time.tm_hour);
	printf("tm_min=%s ", get_time.tm_min);
	printf("tm_sec=%s\n", get_time.tm_sec);
	printf("tm_mday=%s ", get_time.tm_mday);
	printf("tm_mon=%s\n", get_time.tm_mon);
	printf("tm_wday=%s\n", get_time.tm_wday);
	printf("tm_year=%s\n", get_time.tm_year);
		
	
		sleep(1);
	
	return 0;
}
*/