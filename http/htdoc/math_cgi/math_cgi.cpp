#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "BigData.h"
#include <iostream>
using namespace std;

void add(char *data_string)
{
	//data1=100&data2=200
    char *end = data_string + strlen(data_string);
	char *data[2] ;
	int i = 0;

	while(end != data_string)
	{
		if(*end == '=')
		{
			data[i++] = end + 1;
		}
		if(*end == '&')
		{
			*end = '\0';
		}
		--end;
	}

	string num1(data[0]);
	string num2(data[1]);

	BigData big1(num1.c_str()); 
	BigData big2(num2.c_str()); 

	//printf("data1 : %s + data2 : %s = ",data[0],data[1]);
	printf(" %s +  %s =  \n",data[0],data[1]);
	std::cout << big1 + big2 << std::endl;
}

int main()
{
	int content_length = -1;
	char method[1024];
	char query_string[1024];
	char put_data[1024];

	std::cout << "<html>" << std::endl;
	std::cout << "<head> MATH <br/></head> "<< std::endl;
	std::cout << "<body> " << std::endl; 

	strcpy(method,(char *)getenv("REQUEST_METHOD"));

	if(strcasecmp(method,"GET") == 0)
	{
		strcpy(query_string,(char *)getenv("QUERY_STRING"));
		add(query_string);
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
		add(put_data);
	}
	
	std::cout << "</body>" << std::endl;
	std::cout << "</html>" << std::endl;

	return 0;
}

