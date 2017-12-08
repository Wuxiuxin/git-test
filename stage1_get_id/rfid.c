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




/*���ô��ڲ���*/
int init_tty(int fd)
{
	struct termios termios_rfid;
	
	bzero(&termios_rfid, sizeof(termios_rfid));//��սṹ��
	
	cfmakeraw(&termios_rfid);//�����ն����ԣ�����ѡ��
	
	cfsetispeed(&termios_rfid, B9600);//���벨����
	cfsetospeed(&termios_rfid, B9600);//���������
	
	termios_rfid.c_cflag |= CLOCAL | CREAD;//�������Ӻͽ���ʹ��
	
	termios_rfid.c_cflag &= ~CSIZE;//�������λ
	termios_rfid.c_cflag |= CS8;//����λΪ8λ
	
	termios_rfid.c_cflag &= ~PARENB;//����żУ��
	
	termios_rfid.c_cflag &= ~CSTOPB;//һλֹͣλ
	
	tcflush(fd,TCIFLUSH);
	
	termios_rfid.c_cc[VTIME] = 10;//���õȴ�ʱ��
	termios_rfid.c_cc[VMIN] = 1;
	
	tcflush(fd, TCIFLUSH);//������뻺����
	
	if(tcsetattr(fd, TCSANOW, &termios_rfid))//���������
		return 0;
	
	return 1;
}

/*��������*/
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

/*����*/
int PiccRequest(int fd)
{
//	unsigned char WBuf[8], RBuf[8];
	unsigned char WBuf[64], RBuf[64];
	
	int ret;
	
	fd_set fds;
	
	memset(WBuf, 0, 64);
	memset(RBuf, 0, 64);
	
	WBuf[0] = 0x07;//����֡���ȣ������Լ�
	WBuf[1] = 0x02;//�������ͣ�0x02
	WBuf[2] = 0x41;//�����A��
	WBuf[3] = 0x01;//������֡����������Ϣ���ȣ�1
	WBuf[4] = 0x52;//������ϢΪ��0x52
	WBuf[5] = CalBBC(WBuf, WBuf[0]-2);//У���
	WBuf[6] = 0x03;//������־��0x03
	
	FD_ZERO(&fds);//��ռ���
	FD_SET(fd, &fds);//���������
	
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
			if(RBuf[2] == 0x00)//Ӧ��֡״̬����Ϊ0 ������ɹ�
				return 0;
			break;
	}
	
	return -1;
}

/*����ײ����ȡ��Χ�����ID*/

int PiccAnticoll(int fd)
{
//	unsigned char WBuf[8], RBuf[8];
	unsigned char WBuf[64],RBuf[64];
	unsigned long cardid;
	int ret;
	
	fd_set fds;
	
	tcflush(fd, TCIFLUSH);//������뻺����
	
	memset(WBuf, 0, 64);
	memset(RBuf, 0, 64);
	
	WBuf[0] = 0x08;//����֡���ȣ������Լ�
	WBuf[1] = 0x02;//�������ͣ�0x02
	WBuf[2] = 0x42;//����Ϊ����B��
	WBuf[3] = 0x02;//������֡����������Ϣ���ȣ�2
	WBuf[4] = 0x93;//��һ������ײ
	WBuf[5] = 0x00;//λ����=0
	WBuf[6] = CalBBC(WBuf, WBuf[0]-2);//У���
	WBuf[7] = 0x03;//������־��0x03
	
	write(fd, WBuf, 8);
	tcflush(fd, TCIOFLUSH);
	FD_ZERO(&fds);//��ռ���
	FD_SET(fd, &fds);//���������
	
	
	
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
			
			if(RBuf[2] == 0x00)//Ӧ��֡״̬����Ϊ0 ���ȡID �ɹ�
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



