// LogTable  

// ����������ʵ�ֶ�LogTable�ķ��ʲ���  

#ifndef __TABLELOG_H__    
#define __TABLELOG_H__    

#include "DpDatabase.h"  
#include "iostream"
using namespace std;

// ����TableLog�����ݸ�ʽ  
typedef struct
{
	int my_id = NULL;
	string my_alarm = "";
	string my_event = "";
	string my_information = "";
	string my_parameters = "";
}LogInfo;

class TableLog
{
public:
	TableLog();
	virtual ~TableLog();

	void TableLog::TableLogInit(const string &table_name, const string &table_info);// ���Ӵ������ݿ⣬������TableLog  
	bool TableLog::TableLogInsert(LogInfo &log_data);  // ������� 
	vector<LogInfo> TableLog::TableLogQuery(const string &sql);//��������
	bool TableLog::TableLogDelete(const string &table_name, unsigned int id_begin, unsigned int id_end, bool clear_all = false)//��������
		;//ɾ������
	bool TableLog::TableDelete(const string &table_name);//ɾ����
public:
	string table_info;
	LogInfo log_info_instance;   // Log��Ϣ  
	DpSqlInterface dp_sql_intanse; // DpSqlInterfaceʵ��
};

#endif    