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

#include "include/rfid.h"


int write_number(int num)
{
    int num_fd;
    char car_number[4];

    memset(car_number, 0, sizeof(car_number));
    sprintf(car_number, "%d", num);

    num_fd = open("number.txt", O_TRUNC|O_WRONLY);
    if(num_fd == -1)
    {
            perror("W Open number.txt failed!\n");
            close(num_fd);
            exit(1);
    }

    write(num_fd, car_number, sizeof(car_number));


    close(num_fd);

    return 0;
}

int read_number()
{
        
    int ret;
    int  number;
        
    FILE *num_fd;
        
    num_fd = fopen("number.txt", "r");
    if(num_fd == NULL)
    {       
           perror("Open number.txt failed!\n");
            fclose(num_fd);
            exit(1);
    }

    fscanf(num_fd, "%d", &number);

    printf("read_number=%d\n", number);

    fclose(num_fd);
    return number;
}

#if 0
int read_number()
{
	char num[4];
	
	int ret;
	
	FILE *num_fd;
	
	num_fd = fopen("number.txt", "r");
	if(num_fd == NULL)
	{
		perror("Open number.txt failed!\n");
		fclose(num_fd);
		exit(1);
	}
	
	ret = fread(num, 4, 1, num_fd);
	if(ret < 1)
		{
			if(ret == 0)
			{
				printf("R fread number.txt failed!\n");
				fclose(num_fd);
				exit(1);
			}
		}
	
	
	fclose(num_fd);
	return atoi(num);
}


int write_number(char *num)
{
	int num_fd;
	
	//printf("size of num=%d\n", num);
	
	char car_number[4];
	memset(car_number, 0, sizeof(car_number));
	memcpy(car_number, num, sizeof(num));
	
	ssize_t wrtv;
	num_fd = open("number.txt", O_TRUNC|O_WRONLY);
	if(num_fd == -1)
//	if(num_fd != 0)
	{
		perror("W Open number.txt failed!\n");
		close(num_fd);
		exit(1);
	}

	wrtv = write(num_fd, car_number, sizeof(car_number));

	close(num_fd);
	return 0;
}

#endif

