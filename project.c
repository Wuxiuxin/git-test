#include <stdio.h>
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
#include <stdbool.h>

#include "include/rfid.h"

#define SERIAL_PATH1	"/dev/s3c2410_serial1"
#define SERIAL_PATH3	"/dev/s3c2410_serial3"

extern struct timeval timeout;
char cardid_in[9];
char cardid_out[9];

char car_number[4];

extern unsigned long *g_fd_men;
//extern unsigned long *g_fd_men;
pthread_mutex_t car_lock;

void *car_in(void *arg)
{
	unsigned int remain;
	int ret;
	int fd_in;
//	unsigned long cardid;
	unsigned int cardid;
	struct car_get_time in_time;
	car_io_data data;
	
//	listp list = (listp)arg;
	
	regp reg_p = NULL ;
	car_iop car_io_p = NULL;
	
	
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
//			printf("PiccRequest car in failed!\n");
			usleep(500000);
			continue;
		}
		
		tcflush(fd_in, TCIOFLUSH);//清空进车串口IO缓冲区
		usleep(300000);
		
		if((cardid = PiccAnticoll(fd_in)) == 0)
		{
//			printf("PiccAnticoll car in failed!\n");
			usleep(300000);
			continue;
		}
		else
		{
			
			memset(data.name, 0, sizeof(data.name));
			memset(data.id, 0, sizeof(data.id));
			memset(data.time, 0, sizeof(data.time));
			memset(data.mday, 0, sizeof(data.mday));
			memset(data.mon, 0, sizeof(data.mon));
			memset(data.wday, 0, sizeof(data.wday));
			memset(data.year, 0, sizeof(data.year));
			memset(data.charge, 0, sizeof(data.charge));

//			printf("Begin to change to string!-->car in\n");
		//	sprintf(cardid_in, "%ul", cardid); 
			memset(cardid_in, 0 , sizeof(cardid_in));
			sprintf(cardid_in, "%x", cardid); 	
		//	memcpy(data.id, cardid_out+0, 10);
			printf("in card ID = %s\n", cardid_in);

		//	reg_p       = list->reg_head;
		//	car_io_p = list->car_io_head;
			reg_p = ((listp)arg)->reg_head;
			car_io_p = ((listp)arg)->car_io_head;
			
			pthread_mutex_lock(&car_lock);//开启线程锁
			
			system("madplay -Q music/getcard.mp3");
			while( reg_p->next != ((listp)arg)->reg_head)//从链表头开始遍历（reg）
			{
				reg_p = reg_p->next;
				
				
				
				if( !(strncmp(cardid_in, reg_p->data.id, sizeof(cardid_in)-1))  )
				{

				
				//	strcpy(data.name, reg_p->data.name); //name to car_io_data不是VIP卡则没有名字
					memcpy(data.name, reg_p->data.name, sizeof(data.name)-1);								
					
					system("madplay -Q music/huiyuan.mp3");
					system("madplay -Q music/ka.mp3");
					
				//	printf("system(vip)\n");
					break;
	
				}
				else{
					memcpy(data.name, "guest", 5);
				}
			}
			
			car_io_p = car_io_p->prev;
			//同张卡，没有过收费和不同卡都可以进来
			//（&&）解决第二张卡不能进来的问题
			if( strncmp(cardid_in, car_io_p->data.id, sizeof(cardid_in)-1) || 
					strncmp(car_io_p->data.charge, "in_car", sizeof(cardid_in)-1))//
			{
				((listp)arg)->number++;
				
				in_time = car_get_time();
			//	system("madplay music/huiyuan.mp3 1>tmp 2>tmp");
			//	system("madplay music/ka.mp3 1>tmp 2>tmp");
				
			//	memcpy(data.name, reg_p->data.name, sizeof(data.name)-1);
				memcpy(data.id, cardid_in, sizeof(cardid_in)-1);
				
				memcpy(data.time, in_time.tm_time, sizeof(data.time)-1);
				memcpy(data.mday, in_time.tm_mday, sizeof(data.mday)-1);
				memcpy(data.mon, in_time.tm_mon, sizeof(data.mon)-1);
				memcpy(data.wday, in_time.tm_wday, sizeof(data.wday)-1);
				memcpy(data.year, in_time.tm_year, sizeof(data.year)-1);
				memcpy(data.charge, "in_car", sizeof(data.charge)-1);
				
				/*
				strcpy(data.time, in_time.tm_time);
				strcpy(data.mday, in_time.tm_mday);
				strcpy(data.mon, in_time.tm_mon);
				strcpy(data.wday, in_time.tm_wday);
				strcpy(data.year, in_time.tm_year);
				strcpy(data.charge, "in_car");
				*/
		
				car_io_insert(((listp)arg)->car_io_head, data);//
				
				pthread_mutex_unlock(&car_lock);//释放线程锁
				
				remain = NUM - (((listp)arg)->number);
				
				show_chewei(remain);//显示车位

				
				printf("\033[2J");//翻页
				printf("\033[0;0H");
				printf("\033[0;34m");//颜色
			
				show_car_io();
				
				printf("\033[0m");
				
				printf("\033[5;0H");//从顶端相下数五行。
				printf("\033[0;30m");//颜色
				
				printf("%-8s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t\n\n",
				data.name,
				data.id,
				data.time,
				data.mday,
				data.mon,
				data.wday,
				data.year,
				data.charge
				);
				
				printf("剩余车位数: %d\n", remain);
				printf("****************************************************************************************\n");
				printf("****************************************************************************************\n");
		
				
				
				printf("\033[0m");
				if(remain != 0)
					write_number(((listp)arg)->number);
			}
		}
	}
	
		
	tcflush(fd_in, TCIOFLUSH  );

	close(fd_in);
	
	pthread_exit(NULL);
	
}

void *car_out(void *arg)
{
	int ret;
	int fd_out;
//	unsigned long cardid;
	unsigned int cardid;
	struct car_get_time out_time;
	car_io_data data;
	
//	listp list = (listp)arg;
	
	regp reg_p = NULL; //指针初始化为空，好的习惯
	car_iop car_io_p = NULL;
	
		char day_in[3];
		char day_out[3];
		char mon_in[4];
		char mon_out[4];
		char hour_in[3];
		char hour_out[3];
		char charge[7];
					
		int month;
		int day;
		int hour;
		int mony;
	
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
//			printf("PiccRequest car out failed!\n");
			usleep(500000);
			continue;
		}
		
		tcflush(fd_out, TCIOFLUSH);//清空进车串口IO缓冲区
		usleep(300000);
			
		if((cardid = PiccAnticoll(fd_out)) == 0)
		{
//			printf("PiccAnticoll car out failed!\n");
			usleep(300000);
			continue;
		}
		else
		{
//			printf("Begin to change to string!-->car out\n");
		//	sprintf(cardid_out, "%ul", cardid);
			memset(cardid_out, 0 , sizeof(cardid_out));
			sprintf(cardid_out, "%x", cardid);
			printf("out card ID = %s\n", cardid_out);
			
			
			memset(data.name, 0, sizeof(data.name));
			memset(data.id, 0, sizeof(data.id));
			memset(data.time, 0, sizeof(data.time));
			memset(data.mday, 0, sizeof(data.mday));
			memset(data.mon, 0, sizeof(data.mon));
			memset(data.wday, 0, sizeof(data.wday));
			memset(data.year, 0, sizeof(data.year));
			memset(data.charge, 0, sizeof(data.charge));
			
		//	reg_p       = list->reg_head;
		//	car_io_p = list->car_io_head;
			reg_p       = ((listp)arg)->reg_head;
			car_io_p = ((listp)arg)->car_io_head;
			
			pthread_mutex_lock(&car_lock);//开启线程锁
			
			while( reg_p->next != ((listp)arg)->reg_head)//从链表头开始遍历（reg）
			{
				reg_p = reg_p->next;
				
				if( !(strncmp(cardid_out, reg_p->data.id, sizeof(cardid_out)-1))  )
				{
				//	strcpy(data.name, car_io_p->data.name);//不是VIP卡则没有名字
			//		printf("system(vip, 10RMB)\n");
					break;
	
				}			
			}
			
			while(car_io_p->prev != ((listp)arg)->car_io_head)
			{	
				car_io_p = car_io_p->prev;
									
				/*如果是已经收费，则结束遍历，因为如果没有结束，
				会不停地找到上次入库数据, 不停地计费并记录下来*/
				if(!strncmp(cardid_out, car_io_p->data.id, sizeof(cardid_out)-1) && 
						strncmp(car_io_p->data.charge, "in_car", sizeof(cardid_out)-1))
						break;
				
				if( !strncmp(cardid_out, car_io_p->data.id, sizeof(cardid_out)-1) && 
						!(strncmp(car_io_p->data.charge, "in_car", sizeof(cardid_out)-1)))//
				{
				//	system("madplay music/huiyuanka.mp3 1>tmp 2>tmp");	
					
					if( ((listp)arg)->number == NUM)
					{
						//system("madplay manle 1>tmp 2>tmp");
						break;
					}
					
					((listp)arg)->number--;
			
					out_time = car_get_time();
				
				
					memset(day_in, 0, sizeof(day_in));
					memset(day_out, 0, sizeof(day_out));
					memset(mon_in, 0, sizeof(mon_in));
					memset(mon_out, 0, sizeof(mon_out));
					
					memcpy(day_in, car_io_p->data.mday, 2);
					memcpy(day_out, out_time.tm_mday, 2);
					memcpy(mon_in, car_io_p->data.mon, 3);
					memcpy(mon_out, out_time.tm_mon, 3);
					
					day = atoi(day_out) - atoi(day_in); 
					month = atoi(mon_out) - atoi(mon_in);
				
				//	memcpy(hour_in, car_io_p->data.time, 2);
					memcpy(hour_in, car_io_p->data.time+3, 2); //先用分钟来算
					
					
					
					memcpy(hour_out, out_time.tm_time+3, 2);
				
					hour = atoi(hour_out) - atoi(hour_in);
				
					mony = ((month*30 + day)*24 + hour) * 10; //每个钟10块
				
					sprintf(charge, "%d", mony);
					
					
				//	printf("charge = %s\n", charge);
				
				//	strcpy(data.name, reg_p->data.name); //name to car_io_data
				//	strcpy(data.id, reg_p->data.id); //name to car_io_data
					strcpy(data.name, car_io_p->data.name);//不是VIP卡则没有名字
					strcpy(data.id, car_io_p->data.id); //name to car_io_data
			
				//	strcpy(data.time, out_time.tm_time);
					memcpy(data.time, out_time.tm_time, sizeof(data.time)-1);
				//	strcpy(data.mday, out_time.tm_mday);
					memcpy(data.mday, out_time.tm_mday, sizeof(data.mday)-1);
					
					strcpy(data.mon, out_time.tm_mon);
					strcpy(data.wday, out_time.tm_wday);
					strcpy(data.year, out_time.tm_year);
					strcpy(data.charge, charge);
	
					car_io_insert(((listp)arg)->car_io_head, data);//
					
					show_charge(atoi(data.charge));//显示收费
					
					printf("\033[2J");//翻页
					printf("\033[0;0H");
					printf("\033[0;34m");//颜色
				
					show_car_io();
				
			//		printf("\033[0m");
					
					printf("\033[5;0H");//从顶端相下数五行。
					printf("\033[0;30m");//颜色
						printf("%-8s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t\n\n",
						data.name,
						data.id,
						data.time,
						data.mday,
						data.mon,
						data.wday,
						data.year,
						data.charge
						);
					
					printf("收费:%d 元\n", atoi(data.charge));
					printf("****************************************************************************************\n");
					printf("****************************************************************************************\n");
					printf("\033[0m");//取消
					
				//	memset(car_number, 0, sizeof(car_number));
				//	sprintf(car_number, "%s", ((listp)arg)->number);
					
					write_number(((listp)arg)->number); //把已经停车数量保存起来，
					
					pthread_mutex_unlock(&car_lock);//释放线程锁
					
					break;
				} 
		
			}
			
			
		//	memcpy(data.id, cardid_out+0, 10);
		}
		tcflush(fd_out, TCIOFLUSH);
				
				
				
//		printf("out card ID = %s\n", cardid_out);
		
		
	}
	
	close(fd_out);
	pthread_exit(NULL);
	
}


int main(void)
{
	int key, rd, ag;
	pthread_t in, out;
	

	
	lcd_init();
	
	key = admin_test();

	while (key ==0)
	{
	
		login_err_show();
	
		while( getchar() !='\n'); 
	
		scanf("%d", &ag);
		
		if (ag==0)
			return 0;
		
		key=admin_test(); 
	
	}

	
	printf("\033[2J");
	printf("\033[0;0H");
	printf("\033[0;34m");
	printf("*****************************************************\n");
	printf("*                   Enter [0] To Qut                *\n");
	printf("* Name          ID              Type    Tell        *\n");
	printf("*****************************************************\n");	
	printf("\033[0m");
	
	pthread_mutex_init(&car_lock, NULL);//初始化线程锁
	
	listp list = malloc(sizeof(list));
	//初始化链表头
	list->reg_head	   = reg_init();
	list->car_io_head = car_io_init();
	
	lcd_init();//初始化frambuffer
	
	//读取车位数 NUM= 888是总的车位数
	
	list->number = read_number();
	
	show_pic(g_fd_men, 0, 0, 800, 480, "./pic/aodi.bmp");
	
	car_io_read_record(list->car_io_head);
	reg_read_record(list->reg_head);
	
	pthread_create(&in, NULL, &car_in, (void *)list);
	pthread_create(&out, NULL, &car_out, (void*)list);
	
	while(1)
	{
//		printf("find.c\n");
		find(list);
	
	}

	
	
	pthread_mutex_destroy(&car_lock);//销毁线程锁
	pthread_join(in, NULL);
	pthread_join(out, NULL);
	
	lcd_uninit();
	free(list);

	return 0;
}














