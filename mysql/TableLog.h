// LogTable  

// 功能描述：实现对LogTable的访问操作  

#ifndef __TABLELOG_H__    
#define __TABLELOG_H__    

#include "DpDatabase.h"  
#include "iostream"
using namespace std;

// 定义TableLog的数据格式  
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

	void TableLog::TableLogInit(const string &table_name, const string &table_info);// 连接创建数据库，并创建TableLog  
	bool TableLog::TableLogInsert(LogInfo &log_data);  // 添加数据 
	vector<LogInfo> TableLog::TableLogQuery(const string &sql);//查找数据
	bool TableLog::TableLogDelete(const string &table_name, unsigned int id_begin, unsigned int id_end, bool clear_all = false)//查找数据
		;//删除数据
	bool TableLog::TableDelete(const string &table_name);//删除表
public:
	string table_info;
	LogInfo log_info_instance;   // Log信息  
	DpSqlInterface dp_sql_intanse; // DpSqlInterface实例
};

#endif    