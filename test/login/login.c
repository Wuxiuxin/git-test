/*
	1.输入管理员用户名和密码进行验证	
*/
#include <stdio.h>
#include <stdbool.h>

void login_show()				//管理员登录界面
{
	printf("\033[2J");
	printf("\033[0;0H");
	printf("\033[0;34m");
	printf("************************************************\n");
	printf("*    Please Input The Username And Password    *\n");
	printf("************************************************\n");	
	printf("USR:\n");
	printf("PW:\n");
	printf("\033[0m");
}

void login_err_show()
{
	printf("\033[9;0H");
	printf("\033[0;31m");
	printf("   ******************************************\n");
	printf("   *		   Input Error             * \n"); 
	printf("   *        [0]Exit OR [1]Continue         *\n");  
	printf("   ******************************************\n");	
	printf("\033[0m"); 
}

bool check_user_pass(const char *usrname, const char *password)  //文件密码提取与键输入密码验证
{
	char fileName[32];
	char filePass[32];
	char filebuffer[64];

	FILE *file = fopen("password.txt", "r");
	if(file == NULL)
		{
			return false;
		}

	fgets(filebuffer, 64, file);
	
	char *p = filebuffer;

		//获取用户名
	int i = 0;
	while(*p != ':')
		{	
			
			fileName[i] = *p;
			i++;
			p++;
		}
	fileName[i] = '\0';
		//获取密码
	
	i = 0;
	p++;
	while(*p != '#')
	{
		filePass[i] = *p;
		i++;
		p++;
	}
	filePass[i] = '\0';
	//printf("%s:%s\n", fileName, filePass);

	if((strcmp(fileName, usrname) == 0) && (strcmp(filePass, password) == 0))
		{
			fclose(file);
			return true;    
		}
	
	fclose(file);
	return false;
}

int login()				//从键盘获取密码
{
	login_show();
	char inputName[32];
	char inputPass[32];
	
	printf("\033[4;6H");
	
//	while( getchar() !='\n'); 
	scanf("%s", inputName);
	
	printf("\033[5;6H");
	
//	while( getchar() !='\n'); 
	scanf("%s", inputPass);
	
	if(check_user_pass(inputName, inputPass))
		{
			printf("Succes\n");
			return 1;
		}
	else
		{
			printf("Username or Password Error\n");
			return 0;
		}
}
int admin_test(void)		//管理员登录验证
{
	int key;

	
	key=login();

		
	return key;
}

int main(void)
{
	int key, rd;
	int ag;	
	key = admin_test();

	while (key ==0)
	{
	//	start_show();
		login_err_show();
	
		while( getchar() !='\n'); 
	
		scanf("%d", &ag);
		
		if (ag==0)
			return 0;
		
		key=admin_test(); 
	
	}	

	return 0;
}



