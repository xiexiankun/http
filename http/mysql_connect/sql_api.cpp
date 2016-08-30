
#include "sql_api.h"

sql_api* sql_api::obj = NULL;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

sql_api::sql_api(const std::string &_host,const std::string &_user,const std::string &_passwd, const std::string & _db,const  int _port)
	: host(_host)
	  ,user(_user)
	  ,passwd(_passwd)
	  ,db(_db)
	  ,port(_port)
{
	this->conn = mysql_init(NULL);
}

sql_api* sql_api::sql_api_create(const std::string &_host,const std::string &_user,const std::string &_passwd, const std::string & _db,const  int _port)
{
	if (NULL == obj)
	{
		pthread_mutex_lock(&lock);

		if(NULL == obj)
		{
			sql_api* tem  = new sql_api(_host,_user,_passwd, _db, _port);

			//MemeoryBarrier()

			obj = tem;
		}

		pthread_mutex_unlock(&lock);
	}
		
	return obj;
}

int sql_api::my_connect()
{
	mysql_real_connect(conn,host.c_str(),user.c_str(),passwd.c_str(),db.c_str(),port,NULL,0);
	if (!mysql_set_character_set(conn, "utf8"))
	{
	    //printf("New client character set: %s\n",mysql_character_set_name(conn));
	}
}

int sql_api::my_insert(const std::string &table, const std::string &filed, const std::string &values)
{
	std::string sql = "INSERT INTO ";
	sql += table;
	sql += " ";
	sql += filed;
	sql += " ";
	sql += " values ";
	sql += values;
	sql += " ";
	
	int ret	= mysql_query(conn,sql.c_str());
	if (ret == 0)
	{
		std::cout << "INSERT SUCCESS " <<  std::endl;
	}
	
}

int sql_api::my_select(const std::string & table)
{
	std::string sql = "select * from ";
	sql += table;
	sql += " ";

	MYSQL_RES *res;
	MYSQL_ROW row;

	if (mysql_query(conn, sql.c_str()))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
	    exit(0);
	}

	res = mysql_store_result(conn);

	MYSQL_FIELD *fd = NULL;
	int i = 0;
	for(; fd = mysql_fetch_field(res); )
	{
		std::cout << fd->name << "&nbsp;&nbsp;";
	}
	std::cout << "<br/> "<<std::endl;

	while ((row = mysql_fetch_row(res)) != NULL)	
	{
		unsigned int  len = mysql_field_count(conn);
		int i = 0;

		for ( ; i < len ; ++i)
		{
			std::cout << row[i] << " &nbsp;&nbsp; " ;
		}
		std::cout << "<br/>"<< std::endl;
	}

	mysql_free_result(res);
}

sql_api::~sql_api()
{
	mysql_close(conn);
}

//int main()
//{
//	sql_api *sql =  sql_api::sql_api_create();
//
//	sql->my_connect();
//
//	std::string table = "student";
//	std::string filed = "(name, sex,age,school)";
//	std::string values = "('小华', '女',23,'q')";
////	sql->my_insert(table,filed,values);
//
//	sql->my_select(table);
//
//	return 0;
//}



