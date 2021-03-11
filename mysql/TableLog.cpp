#include "TableLog.h"

TableLog::TableLog() 
{
	dp_sql_intanse.SetMySQLConInfo("localhost", "root", "root", "company", 3306);
	if (!dp_sql_intanse.Open()) {
		printf("%d: %s", dp_sql_intanse.ErrorNum, dp_sql_intanse.ErrorInfo);
	}
	if (!dp_sql_intanse.ExistDatabase("company"))
	{
		if (!dp_sql_intanse.CreateDatabase("company")) {
			cout << "创建database 失败" << endl;
		}
	}
	dp_sql_intanse.ChangeDatabase("company");

	table_info = "id INT NOT NULL PRIMARY KEY AUTO_INCREMENT, alarm TIMESTAMP DEFAULT '0000-00-00 00:00:00', \
						event VARCHAR(24),information VARCHAR(100),parameters VARCHAR(24)";
}

TableLog::~TableLog()
{
	dp_sql_intanse.Close();
}

// TableLog初始化  
void TableLog::TableLogInit(const string &table_name, const string &table_info)
{
	if (!dp_sql_intanse.ExistTables("log_table"))
	{
		dp_sql_intanse.CreateTable(table_name, table_info);
		cout << "创建"<< table_name <<"成功!" << endl;
	}
}



// 添加数据  
bool TableLog::TableLogInsert(LogInfo &log_data)
{

	string sql = "INSERT INTO log_table (id, alarm,event,information, parameters ) VALUES (" +
					(log_data.my_id ? to_string(log_data.my_id) : "NULL") + ", " +
					((log_data.my_alarm != "") ? ("\"" + log_data.my_alarm + "\"") : "NULL") + ", " +
					((log_data.my_event != "") ? ("\"" + log_data.my_event + "\"") : "NULL") + ", " +
					((log_data.my_information != "") ? ("\"" + log_data.my_information + "\"") : "NULL") + ", " +
					((log_data.my_parameters != "") ? ("\"" + log_data.my_parameters + "\"") : "NULL") + ")"	;

	cout << sql << endl;
	bool flag = dp_sql_intanse.Query(sql);
	//printf("%d: %s", dp_sql_intanse.ErrorNum, dp_sql_intanse.ErrorInfo);
	return flag;
}

//删除表的数据
bool TableLog::TableLogDelete(const string &table_name, unsigned int id_begin, unsigned int id_end,bool clear_all)//查找数据
{
	bool flag;
	
	if (clear_all)
	{
		flag = dp_sql_intanse.Query("delete from " + table_name);
	}
	else
	{
		string sql = "delete from " + table_name + " where id >= " + to_string(id_begin) + " and id <= " + to_string(id_end);
		flag = dp_sql_intanse.Query(sql);
	}
	return flag;
}

//查找数据
vector<LogInfo> TableLog::TableLogQuery(const string &sql)
{
	vector<vector<string>> my_data;
	dp_sql_intanse.Select(sql, my_data);
	vector<LogInfo> log_datas;
	LogInfo log_data;
	for (int i = 0; i < my_data.size(); i++)
	{
		
		log_data.my_id = atoi(my_data[i][0].c_str());
		log_data.my_alarm = my_data[i][1];
		log_data.my_event = my_data[i][2];
		log_data.my_information = my_data[i][3];
		log_data.my_parameters = my_data[i][4];
		log_datas.push_back(log_data);
	}
	return log_datas;
}

//删除表
bool TableLog::TableDelete(const string &table_name)
{
	bool flag = dp_sql_intanse.Query("DROP TABLE " + table_name);
	//printf("%d: %s", dp_sql_intanse.ErrorNum, dp_sql_intanse.ErrorInfo);
	return flag;
}
