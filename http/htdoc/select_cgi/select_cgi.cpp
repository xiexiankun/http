#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
using namespace std;
#include <string.h>
#include "sql_api.h"

void select(char *data_string)
{
	sql_api *sql = sql_api::sql_api_create();
	
	sql->my_connect();

	std::string table = "student";
	
	sql->my_select(table);
}

int main()
{
	int content_length = -1;
	char method[1024];
	char query_string[1024];
	char put_data[1024];

	std::cout << "<html>" << std::endl;
	std::cout << "<head> SELECT </head><br/>" << std::endl;
	std::cout << "<body> " << std::endl; 

	strcpy(method,(char *)getenv("REQUEST_METHOD"));

	if(strcasecmp(method,"GET") == 0)
	{
		strcpy(query_string,(char *)getenv("QUERY_STRING"));
		select(query_string);
	}
	else if (strcasecmp(method,"POST") == 0)
	{
		content_length = atoi(getenv("CONTENT_LENGTH"));
		size_t i = 0;
		for ( ; i < content_length ; ++i)
		{
			read(0,&put_data[i], 1);
		}

		put_data[i] = '\0';
		select(put_data);
	}

	std::cout << "</body>\n" << std::endl;
	std::cout << "</html>\n" << std::endl;

	return 0;
}



