#ifndef	RFID_H
#define	RFID_H

/*结构体*/

/*
注册过的用户享受VIP优惠，未注册过的用户则没有优惠

*/
typedef	struct reg{	//注册用户结构体
	char name[10];//用户名，默认guest（普通用户）
	char id[11];//ISO14443A卡id号
	char type[8];//vip或average
	char tell[12];
	
}reg_data;

typedef struct car_io{	//进出卡结构体
	char name[10];
	char id[11];
	char time[6];
	int day;
	int year;
	int month;
	
}car_io_data; 

/*链表节点结构体*/
typedef struct reg_node{	//注册用户链表节点结构体
	reg_data data;
	struct reg_node *prev, *next;

}regnode, *regp;

typedef struct car_io_node{		//进出卡链表节点结构体
	car_io_data data;
	struct car_io_node *prev, *next;
	
}car_ionode, *car_iop;




/*rfid.c*/
//extern data_sequence_number(void);
extern init_tty(int fd);
extern PiccRequest(int fd);
extern PiccAnticoll(int fd);

#endif