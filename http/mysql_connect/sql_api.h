#pragma once

#include <iostream>
using namespace std;
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <mysql.h>
#include <pthread.h>
 
extern pthread_mutex_t lock;

class sql_api
{
	public:

		static sql_api * sql_api_create(const std::string &_host = "127.0.0.1",\
					const std::string &_user = "root",const std::string &_passwd = "",\
					const std::string & _db = "test", const int _port = 3306);

		int my_connect();

		int my_insert(const std::string &table, const std::string &filed, const std::string &values);

		int my_select(const std::string & table);

		//int my_close();

		~sql_api();

	private:

		sql_api(const std::string &_host,const std::string &_user,const std::string &_passwd,\
				const std::string & _db, const int _port);

		sql_api(const sql_api& other);

		sql_api& operator = (const sql_api& other);

		static sql_api *obj;

	private:
		MYSQL			*conn;
		std::string		host;
		std::string		user;
		std::string		passwd;
		std::string		db;
		int				port;
};



