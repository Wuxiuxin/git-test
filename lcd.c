#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>

#include "include/rfid.h"

unsigned long *g_fd_men;
int g_lcd_fd;

//图片文件路径数组
char bmp_pic[10][12]={"./pic/0.bmp", "./pic/1.bmp", "./pic/2.bmp", "./pic/3.bmp", 
"./pic/4.bmp", "./pic/5.bmp", "./pic/6.bmp", "./pic/7.bmp", "./pic/8.bmp", "./pic/9.bmp"};


//数字声音文件路径数组							
char music[10][36]={"madplay -Q ./music/0.mp3", "madplay -Q ./music/1.mp3", 
"madplay -Q ./music/2.mp3", "madplay -Q ./music/3.mp3", "madplay -Q ./music/4.mp3",
 "madplay -Q ./music/5.mp3", "madplay -Q ./music/6.mp3", "madplay -Q ./music/7.mp3", 
 "madplay -Q ./music/8.mp3", "madplay -Q ./music/9.mp3"};

int lcd_init(void)
{
	g_lcd_fd = open("/dev/fb0",O_RDWR);
	g_fd_men = (unsigned long *)mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,g_lcd_fd,0);
	if(g_fd_men == NULL)
	{
		printf("mmap g_fd_men failed!\n");
		return -1;
	}
	return 0;
}

int lcd_uninit(void)
{
	munmap(g_fd_men,800*480*4);
}
#if 0
int draw_dot(int x, int y, int color)
{
	
	//*(g_fd_men+(y+i)*800+x+j) = color;
	*(g_fd_men+(y+*800+x) = color;
	return 0;
}
#endif

/*unsigned long *g_fd_men   映射的起始地址
 *int x	  	控件起始坐标x
 *int y   	控件起始坐标y
 *int w   	控件的宽 
 *int h   	控件的高
 *int color 控件的颜色
 *
*/ 
//open(const char *pathname,int flags)  open("up.bmp")
int show_pic(unsigned long *g_fd_men,int x,int y,int w,int h,const char *pathname)
{
	int bmp_fd,i,j;
	char bmp_buffer[w*h*3];
	int lcd_buffer[w*h];
	int X = 0,
		Y = 0;
	
	bmp_fd = open(pathname,O_RDONLY);
	if(bmp_fd == -1)
	{
		printf("open failed!\n");
		return -1;
	}
	lseek(bmp_fd,54,SEEK_SET);
	read(bmp_fd,bmp_buffer,sizeof(bmp_buffer));
	for(i=0;i<w*h;i++)
	{
		lcd_buffer[i] = bmp_buffer[3*i] | bmp_buffer[3*i+1]<<8 | bmp_buffer[3*i+2]<<16; 
	}

	for(i=0;i<h;i++)
		for(j=0;j<w;j++)
		{	
			
			*(g_fd_men+(y+i)*800+x+j) = lcd_buffer[(h-1-i)*w+j];
		}
		
	close(bmp_fd);
	return 0;
}



int show_charge(unsigned int charge)
{
	int ge, shi, bai, qian, wan;
	int tmp = charge;
	ge = tmp%10;
	tmp = tmp/10;
	
	shi = tmp%10;
	tmp = tmp/10;
	
	bai = tmp%10;
	tmp = tmp/10;
	
	qian = tmp%10;
	tmp = tmp/10;
	
	wan = tmp%10;
	tmp = tmp/10;
	
	show_pic(g_fd_men, 0, 0, 800, 480, "./pic/shoufei.bmp");
    show_pic(g_fd_men, 150, 170, 120, 120, bmp_pic[wan]);
    show_pic(g_fd_men, 270, 170, 120, 120, bmp_pic[qian]);
    show_pic(g_fd_men, 390, 170, 120, 120, bmp_pic[bai]);
    show_pic(g_fd_men, 510, 170, 120, 120, bmp_pic[shi]);
    show_pic(g_fd_men, 630, 170, 120, 120, bmp_pic[ge]);

//    show_pic(g_fd_men, 500, 200, 40, 40, "pic/9.bmp");

	system("madplay -Q music/outcard.mp3");
	if(qian != 0)
	{
		system(music[qian]);
		system("madplay -Q ./music/qian.mp3");
	}
	if(bai != 0)
	{
		system(music[bai]);
		system("madplay -Q ./music/bai.mp3");
	}
	if(shi != 0)
	{
		system(music[shi]);
		system("madplay -Q ./music/10.mp3");
	}
	system(music[ge]);
	system("madplay -Q ./music/yuan.mp3");
	

	return 0;
}

int show_chewei(unsigned int number)
{
	int ge, shi, bai;
	
	if(number == 0)
	{
		printf("che wei yi man\n");
		system("madplay -Q ./music/man.mp3");
		return 0;
	}
	
	int tmp = number;
	ge = tmp%10;
	tmp = tmp/10;
	
	shi = tmp%10;
	tmp = tmp/10;
	
	bai = tmp%10;
	tmp = tmp/10;
	
    show_pic(g_fd_men, 0, 0, 800, 480, "pic/chewei.bmp");
    show_pic(g_fd_men, 360, 170, 120, 120, bmp_pic[bai]);
	show_pic(g_fd_men, 480, 170, 120, 120, bmp_pic[shi]);
	show_pic(g_fd_men, 600, 170, 120, 120, bmp_pic[ge]);
	
	
	system("madplay -Q ./music/shengyu.mp3");//利用重定向把打印信息输出到文件，不用输出到PC。	
	if(bai != 0)
	{
		system(music[bai]);
//		system("madplay ./music/bai.mp3");
	}
	if(shi != 0)
	{
		system(music[shi]);
//		system("madplay ./music/shi.mp3");
	}
	system(music[ge]);
	system("madplay -Q ./music/wei.mp3");
	
	return 0;
}









