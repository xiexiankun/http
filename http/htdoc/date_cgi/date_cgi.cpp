#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "CTDate.h"
#include <iostream>
using namespace std;

void dostring(char *str, int *year , int *month, int *day)
{
	// num2:2016-8-2 num1:1995-5-5
	char *end = str + strlen(str);
	char *data[3];
	int i =	0 ;

	while(end != str)
	{
		if(*end == '-')
		{
			data[i++] = end+1;
			*end='\0';
		}
		--end;
	}

	data[2] = str;
	*day = atoi(data[0]);
	*month = atoi(data[1]);
	*year = atoi(data[2]);
}

void date(char *data_string)
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

	// num2:2016-8-2 num1:1995-5-5
	int year = 0;
	int month = 0;
	int day = 0;

	dostring(data[0], &year , &month, &day);
	CTDate date1(year,month,day); 
	std::cout << date1 << std::endl;

	year = 0;
	month = 0;
	day = 0;
	dostring(data[1], &year , &month, &day);
	CTDate date2(year,month,day); 
	std::cout << date2 << std::endl;

	std::cout <<"Nowday ,You Have alive Days :  " ;
	std::cout <<date2 - date1 << std::endl;
}

int main()
{
	int content_length = -1;
	char method[1024];
	char query_string[1024];
	char put_data[1024];

	std::cout << "<html>" << std::endl;
	std::cout << "<head> DATE <br/></head> "<< std::endl;
	std::cout << "<body> " << std::endl; 

	strcpy(method,(char *)getenv("REQUEST_METHOD"));

	if(strcasecmp(method,"GET") == 0)
	{
		strcpy(query_string,(char *)getenv("QUERY_STRING"));
		date(query_string);
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
		date(put_data);
	}
	
	std::cout << "</body>" << std::endl;
	std::cout << "</html>" << std::endl;

	return 0;
}

