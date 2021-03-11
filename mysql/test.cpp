#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<Windows.h>
#include<WinSock.h>
#include<mysql.h>


using namespace std;
#pragma comment(lib,"libmysql.lib")
#pragma comment(lib,"wsock32.lib")
MYSQL *mysql = new MYSQL; //mysql����  
MYSQL_FIELD *fd;    //�ֶ�������  
char field[32][32];    //���ֶ�����ά����  
MYSQL_RES *res; //����ṹ���������е�һ����ѯ�����  
MYSQL_ROW column; //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾ�����е���  
char query[150]; //��ѯ���  

bool ConnectDatabase();
bool QueryDatabase1();
//bool QueryDatabase2();
int main()
{
	ConnectDatabase();
	QueryDatabase1();
	//QueryDatabase2();
	system("pause");
	return 0;
}


bool ConnectDatabase()
{
	//��ʼ��mysql  
	mysql_init(mysql);
	//����false������ʧ�ܣ�����true�����ӳɹ�  
	if (!(mysql_real_connect(mysql, "localhost", "root", "root", "company", 0, NULL, 0))) //�м�ֱ����������û��������룬���ݿ������˿ںţ�����дĬ��0����3306�ȣ���������д�ɲ����ٴ���ȥ  
	{
		printf("Error connecting to database:%s\n", mysql_error(mysql));
		return false;
	}
	else
	{
		printf("Connected...\n");
		return true;
	}
	return true;
}

bool QueryDatabase1()
{
	sprintf_s(query, "select * from t_dept"); //ִ�в�ѯ��䣬�����ǲ�ѯ���У�user�Ǳ��������ü����ţ���strcpyҲ����  
	mysql_query(mysql, "set names gbk"); //���ñ����ʽ��SET NAMES GBKҲ�У�������cmd����������  
										 //����0 ��ѯ�ɹ�������1��ѯʧ��  
	if (mysql_query(mysql, query))    //ִ��SQL���
	{
		printf("Query failed (%s)\n", mysql_error(mysql));
		return false;
	}
	else
	{
		printf("query success\n");
	}
	//��ȡ�����  
	if (!(res = mysql_store_result(mysql)))   //���sql�������󷵻صĽ����  
	{
		printf("Couldn't get result from %s\n", mysql_error(mysql));
		return false;
	}

	//��ӡ��������  
	printf("number of dataline returned: %d\n", mysql_affected_rows(mysql));

	//��ȡ�ֶε���Ϣ  
	char *str_field[32];  //����һ���ַ�������洢�ֶ���Ϣ  
	for (int i = 0; i < 4; i++)  //����֪�ֶ�����������»�ȡ�ֶ���  
	{
		str_field[i] = mysql_fetch_field(res)->name;
	}
	for (int i = 0; i < 4; i++)  //��ӡ�ֶ�  
		printf("%10s\t", str_field[i]);
	printf("\n");
	//��ӡ��ȡ������  
	while (column = mysql_fetch_row(res))   //����֪�ֶ���������£���ȡ����ӡ��һ��  
	{
		printf("%10s\t%10s\t%10s\t%10s\n", column[0], column[1], column[2], column[3]);  //column��������  
	}
	return true;
}