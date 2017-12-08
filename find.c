#include <stdio.h>
#include <string.h>

#include "include/rfid.h"

int find(listp list)
{		
	car_iop p;
	int key;
	int month;
	char ID[9];
	unsigned int count=0;
	
	p = list->car_io_head;
	
		while(1)
		{
				
			while(scanf("%d",&key) != 1 || getchar() != '\n')
			{
			
				while(getchar() != '\n');
				printf("input error,please try again!\n");
			}
			count = 0;
			switch(key)
			{
				case 1:
						printf("\033[2J");//翻页
						printf("\033[0;0H");
						printf("\033[0;34m");//颜色
						
						show_reg();
						reg_show(list->reg_head);
						
						printf("*****************************************************\n");
						
						printf("\033[0m");//取消
				//	printf("vip_show\n");

						break;
				case 2:
						printf("Please Input The Month Number(1):\n");
						
					//	scanf("%s", month);
						
						while(scanf("%d",&month) != 1 || getchar() != '\n')
						{
							if(month == 0) break;
							
							while(getchar() != '\n');
							printf("Input Error, Please Ttry again,\nOr Input 0 To Exit:\n");
						}
						if(month == 0) break;
						
					//	p = list->car_io_head;
						p = p->next;
						
						
						printf("\033[2J");//翻页
						printf("\033[0;0H");
						printf("\033[0;34m");//颜色
						show_car_io();
						
						switch(month)
						{
							case 1:
									while( p != list->car_io_head)
									{	
									if(!(strncmp(p->data.mon, "Jan", sizeof(p->data.mon)-1)) &&
											(strncmp(p->data.charge, "in_car", sizeof(p->data.charge)-1)))
									{ 
										print_info(p);
							
										count += atoi((p->data.charge));	
									}		
									p = p->next;
									}
						
									printf("The total amount of Jan = %d\n", count);
									printf("****************************************************************************************\n\n");

									printf("\033[0m");//取消
									
									break;
							case 2:
									while( p != list->car_io_head)
									{	
									if(!(strncmp(p->data.mon, "Feb", sizeof(p->data.mon)-1)) &&
										(strncmp(p->data.charge, "in_car", sizeof(p->data.charge)-1)))
									{ 
										print_info(p);
							
										count += atoi((p->data.charge));	
									}		
									p = p->next;
									}
						
									printf("The total amount of Feb = %d\n", count);
									printf("****************************************************************************************\n\n");

									printf("\033[0m");//取消
									
									break;
							case 3:
									while( p != list->car_io_head)
									{	
									if(!(strncmp(p->data.mon, "Mar", sizeof(p->data.mon)-1)) &&
										(strncmp(p->data.charge, "in_car", sizeof(p->data.charge)-1)))
									{ 
										print_info(p);
							
										count += atoi((p->data.charge));	
									}		
									p = p->next;
									}
						
									printf("The total amount of Mar = %d\n", count);
									printf("****************************************************************************************\n\n");
									printf("\033[0m");//取消
									break;
							case 4:
									while( p != list->car_io_head)
									{	
									if(!(strncmp(p->data.mon, "Apr", sizeof(p->data.mon)-1)) &&
										(strncmp(p->data.charge, "in_car", sizeof(p->data.charge)-1)))
									{ 
										print_info(p);
							
										count += atoi((p->data.charge));	
									}		
									p = p->next;
									}
						
									printf("The total amount of Apr = %d\n", count);
									printf("****************************************************************************************\n\n");

									printf("\033[0m");//取消
									break;
							
							case 5:
									while( p != list->car_io_head)
									{	
									if(!(strncmp(p->data.mon, "May", sizeof(p->data.mon)-1)) &&
										(strncmp(p->data.charge, "in_car", sizeof(p->data.charge)-1)))
									{ 
										print_info(p);
							
										count += atoi((p->data.charge));	
									}		
									p = p->next;
									}
						
									printf("The total amount of May = %d\n", count);
									printf("****************************************************************************************\n\n");
									printf("\033[0m");//取消
									break;
							case 6:
									while( p != list->car_io_head)
									{	
									if(!(strncmp(p->data.mon, "Jun", sizeof(p->data.mon)-1)) && 
										(strncmp(p->data.charge, "in_car", sizeof(p->data.charge)-1)))
									{ 
										print_info(p);
							
										count += atoi((p->data.charge));	
									}		
									p = p->next;
									}
						
									printf("The total amount of Jun = %d\n", count);
									printf("****************************************************************************************\n\n");
									printf("\033[0m");//取消
									break;
							case 7:
									while( p != list->car_io_head)
									{	
									if(!(strncmp(p->data.mon, "Jul", sizeof(p->data.mon)-1)) &&
										(strncmp(p->data.charge, "in_car", sizeof(p->data.charge)-1)))
									{ 
										print_info(p);
							
										count += atoi((p->data.charge));	
									}		
									p = p->next;
									}
						
									printf("The total amount of Jul = %d\n", count);
									printf("****************************************************************************************\n\n");
									printf("\033[0m");//取消
									break;
							
							case 8:
									while( p != list->car_io_head)
									{	
									if(!(strncmp(p->data.mon, "Aug", sizeof(p->data.mon)-1)) &&
										(strncmp(p->data.charge, "in_car", sizeof(p->data.charge)-1)))
									{ 
										print_info(p);
							
										count += atoi((p->data.charge));	
									}		
									p = p->next;
									}
						
									printf("The total amount of Aug = %d\n", count);
									printf("****************************************************************************************\n\n");
									printf("\033[0m");//取消
									break;
							case 9:
									while( p != list->car_io_head)
									{	
									if(!(strncmp(p->data.mon, "Sep", sizeof(p->data.mon)-1)) &&
									(strncmp(p->data.charge, "in_car", sizeof(p->data.charge)-1)))
									{ 
										print_info(p);
							
										count += atoi((p->data.charge));	
									}		
									p = p->next;
									}
						
									printf("The total amount of Sep = %d\n", count);
									printf("****************************************************************************************\n\n");
									printf("\033[0m");//取消
									break;
							case 10:
									while( p != list->car_io_head)
									{	
									if(!(strncmp(p->data.mon, "Oct", sizeof(p->data.mon)-1)) &&
									(strncmp(p->data.charge, "in_car", sizeof(p->data.charge)-1)))
									{ 
										print_info(p);
							
										count += atoi((p->data.charge));	
									}		
									p = p->next;
									}
						
									printf("The total amount of Oct = %d\n", count);
									printf("****************************************************************************************\n\n");
									printf("\033[0m");//取消
									break;
							case 11:
									while( p != list->car_io_head)
									{	
									if(!(strncmp(p->data.mon, "Nov", sizeof(p->data.mon)-1)) &&
										(strncmp(p->data.charge, "in_car", sizeof(p->data.charge)-1)))
									{ 
										print_info(p);
							
										count += atoi((p->data.charge));	
									}		
									p = p->next;
									}
						
									printf("The total amount of Nov = %d\n", count);
									printf("****************************************************************************************\n\n");
									printf("\033[0m");//取消
									break;	
							case 12:
									while( p != list->car_io_head)
									{	
									if(!(strncmp(p->data.mon, "Dec", sizeof(p->data.mon)-1)) &&
										(strncmp(p->data.charge, "in_car", sizeof(p->data.charge)-1)))
									{ 
										print_info(p);
							
										count += atoi((p->data.charge));	
									}		
									p = p->next;
									}
						
									printf("The total amount of Dec = %d\n", count);
									printf("****************************************************************************************\n\n");
									printf("\033[0m");//取消
									break;
							default:
									while( p != list->car_io_head)
									{	
										if((strncmp(p->data.charge, "in_car", sizeof(p->data.charge)-1)))
										print_info(p);
							
										count += atoi((p->data.charge));			
								
										p = p->next;
									}
									printf("The total amount of this year = %d\n", count);
									printf("****************************************************************************************\n\n");
									printf("\033[0m");//取消
									break;
						}
						
						
#if 0						
						if(!(strncmp(p->data.mon, month, sizeof(p->data.mon)-1)))
						{
							printf("input month\n");
							while( p != list->car_io_head)
							{	
								if(!(strncmp(p->data.mon, month, sizeof(p->data.mon)-1)))
								{ 
									print_info(p);
							
									count += atoi((p->data.charge));	
								}		
								p = p->next;
							}
						
							printf("The total amount of %s = %d", month, count);
						}
						if(!(strncmp("ALL", month, sizeof(p->data.mon)-1)))
						{
						//	printf("input year\n");
							
							while( p != list->car_io_head)
							{	
								print_info(p);
							
								count += atoi((p->data.charge));			
								
								p = p->next;
							}
							printf("The total amount of this year = %d", count);
						}
			
					//	printf("RMB_show  Out\n");
#endif					
						break;

				case 3:
						printf("\033[2J");//翻页
						printf("\033[0;0H");
						printf("\033[0;34m");//颜色
						show_car_io();
						
					//	p = list->car_io_head;
						p = p->next;
						while( p != list->car_io_head)
						{	
							if(!(strncmp(p->data.charge, "in_car", sizeof(p->data.charge)-1)))
								print_info(p);
							
							p = p->next;
						}
						
						printf("****************************************************************************************\n\n");
						
						printf("\033[0m");//取消
						//	printf("IN_show\n");
							break;
	
				case 4:
						printf("Please Input The ID Number:\n");
						while(scanf("%s",ID) != 1 || getchar() != '\n')
						{
							if(!(strncmp(ID, "Q", 1))) break;
							
							while(getchar() != '\n');
							printf("Input Error, Please Ttry again,\nOr Input Q To Exit:\n");
						}
						
						if(!(strncmp(ID, "Q", 1))) break;
						
						printf("\033[2J");//翻页
						printf("\033[0;0H");
						printf("\033[0;34m");//颜色
						show_car_io();
						p = p->next;
						while(p != list->car_io_head)
						{
							if(!strncmp(p->data.id, ID, sizeof(ID)-1) )
								print_info(p);
				
							p = p->next;
						}
						
						printf("****************************************************************************************\n\n");
						printf("\033[0m");//取消
					//	printf("Find_ID\n");
						break;

			//	default: show_menu();
				default: break;
			}
		show_menu();	
		}

//	printf("find.c\n");
	return 0;
}

void print_info(car_iop p)
{
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










