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

/*
#define SERIAL_PATH1	"/dev/s3c2410_serial1"
#define SERIAL_PATH3	"/dev/s3c2410_serial3"
*/
//volatile unsigned int cardid;
struct timeval timeout;




/*设置串口参数*/
int init_tty(int fd)
{
	struct termios termios_rfid;
	
	bzero(&termios_rfid, sizeof(termios_rfid));//清空结构体
	
	cfmakeraw(&termios_rfid);//设置终端属性，激活选项
	
	cfsetispeed(&termios_rfid, B9600);//输入波特率
	cfsetospeed(&termios_rfid, B9600);//输出波特率
	
	termios_rfid.c_cflag |= CLOCAL | CREAD;//本地连接和接收使能
	
	termios_rfid.c_cflag &= ~CSIZE;//清空数据位
	termios_rfid.c_cflag |= CS8;//数据位为8位
	
	termios_rfid.c_cflag &= ~PARENB;//无奇偶校验
	
	termios_rfid.c_cflag &= ~CSTOPB;//一位停止位
	
	tcflush(fd,TCIFLUSH);
	
	termios_rfid.c_cc[VTIME] = 10;//设置等待时间
	termios_rfid.c_cc[VMIN] = 1;
	
	tcflush(fd, TCIFLUSH);//清空输入缓冲区
	
	if(tcsetattr(fd, TCSANOW, &termios_rfid))//激活串口设置
		return 0;
	
	return 1;
}

/*计算检验和*/
unsigned char CalBBC(unsigned char *buf, int n)
{
	int i;
	
	unsigned char bcc = 0;
	for(i=0; i<n; i++)
	{
		bcc ^= *(buf+i);
	}
	
	return (~bcc);
}

/*请求*/
int PiccRequest(int fd)
{
//	unsigned char WBuf[8], RBuf[8];
	unsigned char WBuf[64], RBuf[64];
	
	int ret;
	
	fd_set fds;
	
	memset(WBuf, 0, 64);
	memset(RBuf, 0, 64);
	
	WBuf[0] = 0x07;//数据帧长度，包含自己
	WBuf[1] = 0x02;//命令类型：0x02
	WBuf[2] = 0x41;//命令：“A”
	WBuf[3] = 0x01;//该数据帧所带数据信息长度：1
	WBuf[4] = 0x52;//数据信息为：0x52
	WBuf[5] = CalBBC(WBuf, WBuf[0]-2);//校验和
	WBuf[6] = 0x03;//结束标志：0x03
	
	FD_ZERO(&fds);//清空集合
	FD_SET(fd, &fds);//添加描述符
	
	write(fd, WBuf, 7);
	
	ret = select(fd+1, &fds, NULL, NULL, &timeout);
	
	switch(ret)
	{
		case -1:
			printf("select error!\n");
			break;
		case  0:
			printf("request time out!\n");
		default:
			ret = read(fd, RBuf, 8);
			if(ret < 0)
			{
				printf("read ret = %d\n", ret);
				break;
			}
			if(RBuf[2] == 0x00)//应答帧状态部分为0 则请求成功
				return 0;
			break;
	}
	
	return -1;
}

/*防碰撞，获取范围内最大ID*/

int PiccAnticoll(int fd)
{
//	unsigned char WBuf[8], RBuf[8];
	unsigned char WBuf[64],RBuf[64];
	unsigned long cardid;
	int ret;
	
	fd_set fds;
	
	tcflush(fd, TCIFLUSH);//清空输入缓冲区
	
	memset(WBuf, 0, 64);
	memset(RBuf, 0, 64);
	
	WBuf[0] = 0x08;//数据帧长度，包含自己
	WBuf[1] = 0x02;//命令类型：0x02
	WBuf[2] = 0x42;//命令为：“B”
	WBuf[3] = 0x02;//该数据帧所带数据信息长度：2
	WBuf[4] = 0x93;//第一级防碰撞
	WBuf[5] = 0x00;//位计数=0
	WBuf[6] = CalBBC(WBuf, WBuf[0]-2);//校验和
	WBuf[7] = 0x03;//结束标志：0x03
	
	write(fd, WBuf, 8);
	tcflush(fd, TCIOFLUSH);
	FD_ZERO(&fds);//清空集合
	FD_SET(fd, &fds);//添加描述符
	
	
	
	ret = select(fd+1, &fds, NULL, NULL, &timeout);
	usleep(300000);
	switch(ret)
	{
		case -1:
			printf("select error!\n");
			break;
		case  0:
			printf("request time out!\n");
		default:
			ret = read(fd, RBuf, 10);//ret = read(fd, RBuf, 8);
			tcflush(fd, TCIOFLUSH);
			
			if(ret < 0)
			{
				printf("read ret = %d\n", ret);
				break;
			}
			printf("Before answer!\n");
			
			if(RBuf[2] == 0x00)//应答帧状态部分为0 则获取ID 成功
			{	int i;
			
				for(i=9; i>=0; i--)
				{
					printf("%x!\n", RBuf[i]);
				}
				printf("Answer success!\n");
				cardid = (RBuf[7]<<24) | (RBuf[6]<<16) | (RBuf[5]<<8) | RBuf[4];
				printf("Get id finish->%x!\n", cardid);
			
						return cardid;
			}
			break;
	}
	
	return 0;
	
}



