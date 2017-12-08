#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include <stdlib.h> //malloc
#include <errno.h>

#include "./include/rfid.h"

regp reg_init(void)
{
	regp head = malloc(sizeof(regnode));
	
	if(head == NULL)
	{
		printf("Init reg list failed!\n");
		exit(1);
	}
	
	head->next = head;	//让表头的上一个和下一个节点都指向自己，实现初始化。
	head->prev = head;
	
	return head;
}

void reg_insert_list(regp head, reg_data data)
{
	
	regp new = malloc(sizeof(regnode));
	
	if(new == NULL)
	{
		printf("Create reg node failed!\n");
		return ;
	}
	
	new->data = data;
	
	regp p = head->next;
	while(p != head)
	{
		
		p = p->next;
	}
	
	new->next = p;
	new->prev = p->prev;
	p->prev->next = new;
	p->prev = new;
	
}

void reg_show(regp head)
{
	regp p = head;
	
	while(p->next != head)
	{
		p = p->next;
		printf("%-8s\t%s\t%s\t%s\n",
		p -> data.name,
		p -> data.id,
		p -> data.type,
		p -> data.tell
		);
	}	
	printf("\n");
	
}

void clean_reg_list(regp head)
{
	regp tmp;
	regp p = head->next;
	
	while(p != head)	//从链表头的下一个开始remove
	{
		tmp = p->next;		
		p->next->prev = p->prev;
		p->prev->next = p->next;
		free(p);
		p = tmp;
	}
	
}

void reg_read_record(regp head)
{
//	printf("now in reg_add_record!\n");
	FILE *reg_fp = fopen("reg.txt", "r");
	
	if(reg_fp == NULL)
	{
		perror("Open reg.txt failed!\n");
		exit(1);
	}
	
	int i;
	reg_data data;
	
	char buf[sizeof(reg_data)];
	
	memset(buf, 0, sizeof(reg_data));
	
	for(i=0; fgets(buf, sizeof(reg_data), reg_fp) != NULL; i++)
	{
		sscanf(buf, "%s%s%s%s",
			data.name,
			data.id,
			data.type,
			data.tell
		);
	
	reg_insert_list(head, data);
		
	}
	
//	reg_show(head);
	
//	printf("%d reg items have been read.\n", i);
	
	fclose(reg_fp);
	
}


















