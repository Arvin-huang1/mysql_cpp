#include "DpDatabase.h"
#include "iostream"
#include "TableLog.h"
using namespace std;


int main()
{
	TableLog table_log;
	table_log.TableLogInit("log_table", table_log.table_info);
	LogInfo info;
	info.my_id = 1;
	info.my_event = "啊";
	info.my_information = "tewwst";
	info.my_parameters = "asd";
	DpSqlInterface dp;
	
	if (table_log.TableLogInsert(info))
	{
		cout << "添加数据成功" << endl;
	}
	
	vector<LogInfo> log_infos = table_log.TableLogQuery("select * from log_table");

	for (int i = 0; i < log_infos.size(); i++)
	{
		cout << log_infos[i].my_id << endl;
		cout << log_infos[i].my_alarm << endl;
		cout << log_infos[i].my_event << endl;
		cout << log_infos[i].my_information << endl;
		cout << log_infos[i].my_parameters << endl;
		cout << "============================================" << endl;
	}

	
	table_log.TableLogDelete("log_table", 1, 2,true);

	system("pause");
	return 0;

}

#if 0
DpSqlInterface g_db_obj;
g_db_obj.SetMySQLConInfo("localhost", "root", "root", "company", 3306);
if (!g_db_obj.Open()) {
	printf("%d: %s", g_db_obj.ErrorNum, g_db_obj.ErrorInfo);
}
if (!g_db_obj.ExistDatabase("company"))
{
	if (!g_db_obj.CreateDatabase("company")) {
		cout << "创建database 失败" << endl;
	}
}
g_db_obj.ChangeDatabase("company");

if (!g_db_obj.ExistTables("log_table"))
{
	string str = "id INT NOT NULL PRIMARY KEY AUTO_INCREMENT, alarm TIMESTAMP DEFAULT '0000-00-00 00:00:00', \
enent VARCHAR(24),information VARCHAR(100),parameters VARCHAR(24)";
	g_db_obj.CreateTable("log_table", str);
	cout << "创建log_table成功" << endl;
}
/*
string sql = "INSERT INTO log_table (id, alarm) VALUES ('0', '2021-03-10 17:56:00'); ";
if (g_db_obj.Query(sql))
{
cout << "插入数据成功。" << endl;
}
*/

vector<std::vector<std::string> > data;
g_db_obj.Select("SELECT * FROM log_table", data);

g_db_obj.Close();
#endif