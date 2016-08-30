#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include "sql_api.h"

void insert(char *data_string)
{
	sql_api *sql = sql_api::sql_api_create();
	
	sql->my_connect();

	char *buf[4];

	char *end = data_string + strlen(data_string);

	int index = 0;

	while(end != data_string)
	{
		if(*end == '=')
		{
			buf[index++] = end+1;
		}
		if(*end == '&')
		{
			*end = '\0';
		}
		--end;
	}

	printf("name :  %s \n",buf[3]);
	printf("sex :  %s \n",buf[2]);
	printf("age :  %s \n",buf[1]);
	printf("school : %s <br/> \n",buf[0]);

	std::string table = "student";
	std::string filed = "(name,sex,age,school)";
	
	std::string values = "(' ";
	values += buf[3];
	values += " ' , '";
	values += buf[2];
	values += "' ,  ";
	values += buf[1];
	values += " , ' ";
	values += buf[0];
	values += " ' )";

	sql->my_insert(table,filed,values);
}

int main()
{
	int content_length = -1;
	char method[1024];
	char query_string[1024];
	char put_data[1024];

	printf("<html>\n");
	printf("<head> INSERT \n");
	printf("<meta http-equiv=\"Content-Type\" content=\"text/html\"; charset=\"utf-8\" /> <br/>");
	printf("</head> <br/>");

	printf("<body>\n");

	strcpy(method,(char *)getenv("REQUEST_METHOD"));

	if(strcasecmp(method,"GET") == 0)
	{
		strcpy(query_string,(char *)getenv("QUERY_STRING"));
		insert(query_string);
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
		insert(put_data);
	}

	printf("</body>\n");
	printf("</html>\n");

	return 0;
}


