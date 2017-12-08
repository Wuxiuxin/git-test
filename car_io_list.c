#include <stdio.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "./include/rfid.h"

car_iop car_io_init(void)
{
	car_iop head = malloc(sizeof(car_ionode));
	
	if(head == NULL)
	{
		printf("Init car_io list failed!\n");
		exit(1);
	}
	
	head->next = head;
	head->prev = head;
	
	return head;
}
//有写进文本
void car_io_insert(car_iop head, car_io_data data)
{
	
	int i;
	int ret;
	int wrtv;
	FILE *car_io_fp;
	car_iop new = malloc(sizeof(car_ionode));
	
	if(new == NULL)
	{
		printf("Create car_io node failed!\n");
		return;
	}
	
	new->data = data;
	
	car_iop p = head->next;
	
	while(p != head)
	{
		p = p->next;
	}
	
	new->next = p;
	new->prev = p->prev;
	p->prev->next = new;
	p->prev = new;
	
//	car_io_fp = fopen("car_io.txt", "a+w");
	car_io_fp = fopen("car_io.txt", "a+");
	if(car_io_fp == NULL)
	{
		perror("Open car_io.txt failed!\n");
	}
//	fseek(car_io_fp, SEEK_END, 0);
	
	wrtv = fwrite(&data, sizeof(car_io_data), 1, car_io_fp);
	
	fclose(car_io_fp);

}


//无写进文本
void car_io_insert_list(car_iop head, car_io_data data)
{
	
	int i;
	int wrtv;
	car_iop new = malloc(sizeof(car_ionode));
	
	if(new == NULL)
	{
		printf("Create car_io node failed!\n");
		return;
	}
	
	new->data = data;
	
	car_iop p = head->next;
	
	while(p != head)
	{
		p = p->next;
	}
	
	new->next = p;
	new->prev = p->prev;
	p->prev->next = new;
	p->prev = new;
}

void car_io_show(car_iop head)
{
	car_iop p = head;
	
	while(p->next != head)
	{
		p = p->next;
		
		printf("%-8s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t\n",
		p->data.name,
		p->data.id,
		p->data.time,
		p->data.mday,
		p->data.mon,
		p->data.wday,
		p->data.year,
		p->data.charge
		);
	}
	printf("\n");
}

void clean_car_io_list(car_iop head)
{
	car_iop tmp;
	car_iop p = head->next;
	
	while(p != head)
	{
		tmp = p->next;
	
		p->next->prev = p->next;
		p->prev->next = p->next;
		free(p);
		p = tmp;
	}
	
}

int car_io_read_record(car_iop head)
{
	int i;
	size_t ret;
	int rrtv;
	FILE *car_io_fp = fopen("car_io.txt", "r");
	
	if(car_io_fp == NULL)
	{
		perror("Open car_io.txt failed!\n");
		exit(1);
	}
	
	
	car_io_data data;
	
	char buf[sizeof(car_io_data)];
	
	memset(buf, 0, sizeof(car_io_data));
	
	while(1)
	{	
		//name
		ret = fread(data.name, sizeof(data.name), 1, car_io_fp);
		if(ret < 1)
		{
			if(feof(car_io_fp) == 1)
				break;
			
			if(ret == 0)
			{
				printf("fread car_io.txt failed!\n");
				exit(1);
			}
		}
		//id
		ret = fread(data.id, sizeof(data.id), 1, car_io_fp);
		if(ret < 1)
		{
			if(feof(car_io_fp) == 1)
				break;
			
			if(ret == 0)
			{
				printf("fread car_io.txt failed!\n");
				exit(1);
			}
		}
		//time
		ret = fread(data.time, sizeof(data.time), 1, car_io_fp);
		if(ret < 1)
		{
			if(feof(car_io_fp) == 1)
				break;
			
			if(ret == 0)
			{
				printf("fread car_io.txt failed!\n");
				exit(1);
			}
		}
		//mday
		ret = fread(data.mday, sizeof(data.mday), 1, car_io_fp);
	
		if(ret < 1)
		{
			if(feof(car_io_fp) == 1)
				break;
			
			if(ret == 0)
			{
				printf("fread car_io.txt failed!\n");
				exit(1);
			}
		}
		//mon
		ret = fread(data.mon, sizeof(data.mon), 1, car_io_fp);
		if(ret < 1)
		{
			if(feof(car_io_fp) == 1)
				break;
			
			if(ret == 0)
			{
				printf("fread car_io.txt failed!\n");
				exit(1);
			}
		}
		//wday
		ret = fread(data.wday, sizeof(data.wday), 1, car_io_fp);
		if(ret < 1)
		{
			if(feof(car_io_fp) == 1)
				break;
			
			if(ret == 0)
			{
				printf("fread car_io.txt failed!\n");
				exit(1);
			}
		}
		//year 
		ret = fread(data.year, sizeof(data.year), 1, car_io_fp);
		if(ret < 1)
		{
			if(feof(car_io_fp) == 1)
				break;
			
			if(ret == 0)
			{
				printf("fread car_io.txt failed!\n");
				exit(1);
			}
		}
		//charge 
		ret = fread(data.charge, sizeof(data.charge), 1, car_io_fp);
		if(ret < 1)
		{
			if(feof(car_io_fp) == 1)
				break;
			
			if(ret == 0)
			{
				printf("fread car_io.txt failed!\n");
				exit(1);
			}
		}	
/*			
		printf("before change\n");
		sscanf(buf, "%s%s%s%d%d%d", 
			data.name,
			data.id,
			data.time,
			&data.day,
			&data.year,
			&data.month
		);
*/	
		car_io_insert_list(head, data);
	
		i++;	
	}
//	car_io_show(head);
	
	fclose(car_io_fp);
	return i;
}


