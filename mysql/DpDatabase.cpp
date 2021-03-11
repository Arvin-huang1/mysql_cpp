//#include "stdafx.h"
#include "DpDatabase.h"
DpSqlInterface::DpSqlInterface() :
ErrorNum(0), ErrorInfo("ok")
{
	
	IsOpened = false;
}

DpSqlInterface::~DpSqlInterface()
{
}

// 设置连接信息  
void DpSqlInterface::SetMySQLConInfo(char* server, char* username, char* password, char* database, int port)
{
	MysqlConInfo.server = server;
	MysqlConInfo.user = username;
	MysqlConInfo.password = password;
	MysqlConInfo.database = database;
	MysqlConInfo.port = port;
}

// 打开连接  
bool DpSqlInterface::Open()
{
    mysql_library_init(0, NULL, NULL);
    mysql_init(&MysqlInstance);

    // 设置字符集，否则无法处理中文  
   // mysql_options(&MysqlInstance, MYSQL_SET_CHARSET_NAME, "gbk");
    IsOpened = false;
    unsigned int timeout = 5;

    int ret = mysql_options(&MysqlInstance, MYSQL_OPT_CONNECT_TIMEOUT, (const char *)&timeout);
    if (ret){
        ErrorIntoMySQL();
        return false;
    }
	if (mysql_real_connect(&MysqlInstance, MysqlConInfo.server, MysqlConInfo.user,
		MysqlConInfo.password, MysqlConInfo.database, MysqlConInfo.port, 0, 0) != NULL)
	{
		IsOpened = true;
		return true;
	}
	else
	{
		ErrorIntoMySQL();
		return false;
	}
}

// 断开连接  
void DpSqlInterface::Close()
{
	mysql_close(&MysqlInstance);
	IsOpened = false;
}

//读取数据  
bool DpSqlInterface::Select(const std::string& Querystr, std::vector<std::vector<std::string> >& data)
{
	if (0 != mysql_query(&MysqlInstance, Querystr.c_str()))
	{
		ErrorIntoMySQL();
		return false;
	}

	Result = mysql_store_result(&MysqlInstance);

	// 行列数  
	my_ulonglong row = mysql_num_rows(Result);
	UINT field = mysql_num_fields(Result);

	MYSQL_ROW line = NULL;
	line = mysql_fetch_row(Result);

	int j = 0;
	std::string temp;
	std::vector<std::vector<std::string> >().swap(data);
	while (NULL != line)
	{
		std::vector<std::string> linedata;
		for (UINT i = 0; i < field; i++)
		{
			if (line[i])
			{
				temp = line[i];
				linedata.push_back(temp);
			}
			else
			{
				temp = "";
				linedata.push_back(temp);
			}
		}
		line = mysql_fetch_row(Result);
		data.push_back(linedata);
	}
	return true;
}

// 其他操作  
bool DpSqlInterface::Query(const std::string& Querystr)
{
	if (0 == mysql_query(&MysqlInstance, Querystr.c_str()))
	{
		return true;
	}
	ErrorIntoMySQL();
	return false;
}

// 插入并获取插入的ID,针对自动递增ID  
my_ulonglong DpSqlInterface::GetInsertID(const std::string& Querystr)
{
	if (!Query(Querystr))
	{
		ErrorIntoMySQL();
		return ERROR_QUERY_FAIL;
	}
	// 获取ID  
	return mysql_insert_id(&MysqlInstance);
}




//错误信息  
void DpSqlInterface::ErrorIntoMySQL()
{
	ErrorNum = mysql_errno(&MysqlInstance);
	ErrorInfo = mysql_error(&MysqlInstance);
}
bool DpSqlInterface::ExistDatabase(const char* pDbName)
{
	char CmdArr[256];
	strcpy_s(CmdArr, "SELECT * FROM information_schema.SCHEMATA where SCHEMA_NAME = '");
	strcat_s(CmdArr, pDbName);
	strcat_s(CmdArr, "';");
	mysql_query(&MysqlInstance, CmdArr);
	Result = mysql_store_result(&MysqlInstance);
	MYSQL_ROW record = mysql_fetch_row(Result);
	if (record == NULL)
		return false;
	if (strcmp(record[0], "def") == 0)//版本低时，record[0] == NULL
		return true;
	else
		return false;
	
}
bool DpSqlInterface::ExistTables(const char* pTableName)
{
	char CmdArr[256];
	strcpy_s(CmdArr, "SELECT table_name FROM information_schema.TABLES WHERE table_name = '");
	strcat_s(CmdArr, pTableName);
	strcat_s(CmdArr, "';");
	int Ret = mysql_query(&MysqlInstance, CmdArr);
	Result = mysql_store_result(&MysqlInstance);
	MYSQL_ROW record = mysql_fetch_row(Result);
	
	if (record != 0)
		return true;
	else
		return false;

}
bool DpSqlInterface::CreateDatabase(const std::string sDbName)
{
	std::string cmdCode = "CREATE DATABASE ";
	cmdCode = cmdCode + sDbName;
	if (mysql_query(&MysqlInstance, cmdCode.c_str()) == 0)
	{
		return true;
	}
	else{
		ErrorIntoMySQL();
		return false;
	}
}
bool DpSqlInterface::ChangeDatabase(const std::string sDbName)
{
	std::string cmdCode = "USE ";
	cmdCode = cmdCode + sDbName + ";";
	if (mysql_query(&MysqlInstance, cmdCode.c_str()) == 0)
	{
		return true;
	}
	else{
		ErrorIntoMySQL();
		return false;
	}
}
bool DpSqlInterface::CreateTable(const std::string sTableName, const std::string sFieldName)
{
	std::string sTable = "CREATE TABLE " + sTableName+ " (" + sFieldName + ")";
	if (mysql_query(&MysqlInstance, sTable.c_str()) == 0)
	{
		return true;
	}
	else{
		ErrorIntoMySQL();
		return false;
	}
}