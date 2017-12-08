#ifndef	RFID_H
#define	RFID_H


#define NUM 888
/*结构体*/

/*
注册过的用户享受VIP优惠，未注册过的用户则没有优惠

*/
typedef	struct reg{	//注册用户结构体
	char name[10];//用户名，默认guest（普通用户）
	char id[9];//ISO14443A卡id号 "ffff ffff"
	char type[8];//vip或average
	char tell[12];
	
}reg_data;

typedef struct car_io{	//进出卡结构体
	char name[10];
	char id[9]; 
	char time[9];
	char mday[3];
	char mon[4];
	char wday[4];
	char year[5];
	char charge[7];
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

typedef struct list_t{
	regp 	reg_head;
	car_iop	car_io_head;
	int	number;
//volatile int number;	进程中共享的变量必须用关键字volatile来定义，
						//这是为了防止编译器在优化时（如gcc中使用-OX参数）
						//改变它们的使用方式。
}list_t, *listp;


//time.c
struct car_get_time{
	char tm_sec[3];
	char tm_min[3];
	char tm_hour[3];
	char tm_time[9];
	char tm_mday[3];
	char tm_mon[4];
	char tm_wday[4];
	char tm_year[5];
	char tm_date[25];
}; 
struct car_get_time car_get_time();

//car_io_list.c//进出库链表
car_iop car_io_init(void);
void car_io_insert(car_iop head, car_io_data data);//有写入
void car_io_insert_list(car_iop head, car_io_data data);//无写入
void car_io_show(car_iop head);
void clean_car_io_list(car_iop head);
int car_io_read_record(car_iop head);
//从链表尾开始遍历
/*
#define list_for_each_prev(pos, head) \
for (pos = (head)->prev; pos != (head); \
pos=pos->prev)
*/

//reg_list.c//
regp reg_init(void);
void reg_insert_list(regp head, reg_data data);//无写入
void reg_show(regp head);
void clean_reg_list(regp head);
void reg_read_record(regp head);


//rfid.c
int init_tty(int fd);
int PiccRequest(int fd);
int PiccAnticoll(int fd);
void show_menu();
void show_car_io();
void show_reg();




//lcd.c
int show_pic(unsigned long *g_fd_men,int x,int y,int w,int h,const char *pathname);
int lcd_init(void);
int lcd_uninit(void);
int show_charge(unsigned int charge);
int show_chewei(unsigned int number);


//login.c 
void login_show();
void login_err_show();
int check_user_pass(const char *usrname, const char *password);  //文件密码提取与键输入密码验证
int login();				//从键盘获取密码
int admin_test(void);		//管理员登录验证


//find.c      
void print_info(car_iop p);
int find(listp list);


//number.c
int read_number();
int write_number(int num);


#endif
