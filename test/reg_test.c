#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include "include/rfid.h"

int main(void)
{	
	printf("reg_list test!\n");
	
	regp head = reg_init();
	
	reg_add_record(head);
	reg_show(head);
	
	
	return 0;
}