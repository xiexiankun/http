#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>         
#include <sys/select.h>
#include <sys/time.h>   
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <sys/wait.h>

#define _SIZE_ 1024

static void usage(const char*proc)
{
	printf("Usage : %s [IP] [PORT] \n",proc);
}

static int startup(const char *_ip , int _port)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("socket");
		exit(2);
	}

	int opt = 1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));	

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(_port);
	local.sin_addr.s_addr = inet_addr(_ip);

	if(bind(sock,(struct sockaddr *)&local,sizeof(local)) < 0)
	{
		perror("bind");
		exit(3);
	}

	if(listen(sock,4) < 0)
	{
		perror("listen");
		exit(4);
	}
	return sock;
}
	
static int get_line(int sock,char buf[], int len)
{
	if(NULL == buf || len < 0)
		return -1;

	char ch = '\0';
	int n = recv(sock, &ch, 1, 0);
	int index = 0;

	while(index < (len-1) &&  n > 0 && ch != '\n')
	{
		if(ch == '\r')
		{
			n = recv(sock, &ch, 1, MSG_PEEK);
			if(n > 0 && ch != '\n')
			{
				recv(sock, &ch, 1, 0);
			}
			else
			{
				ch = '\n'; 
			}
		}
		buf[index++] = ch;
		n = recv(sock, &ch, 1, 0);
	}

	buf[index] = '\0';
	
	return index;
}

void echo_error(int sock,int status)
{
	char buf[_SIZE_];
	memset(buf,'\0',sizeof(buf));

    switch(status)
	{
		case 404:
			strcpy(buf,"404 NOT FIND");
			send(sock,buf,strlen(buf),0);
			break;
		case 500:
			strcpy(buf,"500 SERVER ERROR");
			send(sock,buf,strlen(buf),0);
			break;
		case 503:
			strcpy(buf,"503 SERVER DO NOT CLIENT REQUEST,PLEASE WAIT....");
			send(sock,buf,strlen(buf),0);
			break;
	}
}

void clear_header(int sock)
{
	char buf[_SIZE_];
	memset(buf,'\0',sizeof(buf));
	int ret  = -1;

	do
	{
		ret = get_line(sock,buf,sizeof(buf));
	}while(ret > 0 && strcmp(buf,"\n") != 0);
}


void exec(int sock,const char *path,const char *method,const char *query_string)
{
	char buf[_SIZE_];
	int ret = -1;
	int content_length = -1;
	int cgi_input[2] = {0,0};
	int cgi_output[2] = {0,0};

	if(strcasecmp(method,"GET") == 0)
	{
		clear_header(sock);
	}
	else
	{
		do
		{
			ret = get_line(sock,buf,sizeof(buf));

//			if(strncasecmp(buf,"Content-Length:",16) == 0) // 16 error ; should be 15
//			{
//				content_length = atoi(&buf[16]);
//			}
//
			if(strncasecmp(buf, "Content-Length:", strlen("Content-Length:")) == 0)
			{
				content_length = atoi(&buf[16]);
			}
		
		}while(ret > 0 && strcmp(buf,"\n") != 0);

		if(content_length == -1)
		{
			echo_error(sock,500);
			return ;
		}
	}

//	printf("method :%s\n",method);
//	printf("query string :%s\n",query_string);
//	printf("content_lenght :%d\n",content_length);

	char buf1[_SIZE_] = "HTTP/1.1 200 OK \r\n\r\n";
	send(sock,buf1,strlen(buf1),0);

	if(pipe(cgi_input) < 0)
	{
		perror("pipe(cgi_input)");	
		echo_error(sock,500);
		return ;
	}
	if(pipe(cgi_output) < 0)
	{
		perror("pipe(cgi_input)");	
		echo_error(sock,500);
		close(cgi_input[0]);
		close(cgi_input[1]);
		return ;
	}

	pid_t pid = -1;
	pid = fork();
	if(pid < 0)
	{
		perror("fork");	
		close(cgi_input[0]);
		close(cgi_input[1]);
		close(cgi_output[0]);
		close(cgi_output[1]);
		echo_error(sock,503);
		return ;
	}
	if(pid == 0)
	{
		char method_env[_SIZE_];
		char query_string_env[_SIZE_];
		char length_env[_SIZE_];
		memset(method_env,'\0',sizeof(method_env));
		memset(query_string_env,'\0',sizeof(query_string_env));
		memset(length_env,'\0',sizeof(length_env));


		close(cgi_input[1]);
		close(cgi_output[0]);

		dup2(cgi_input[0],0);
		dup2(cgi_output[1],1); //cgi write

		sprintf(method_env,"REQUEST_METHOD=%s",method);
		putenv(method_env);

		if(strcasecmp(method,"GET") == 0)
		{
			sprintf(query_string_env,"QUERY_STRING=%s",query_string);
			putenv(query_string_env);
		}
		else
		{
			sprintf(length_env,"CONTENT_LENGTH=%d",content_length);
			putenv(length_env);
		}

		execl(path,path,NULL);

		exit(-1);
	}
	else
	{
		close(cgi_input[0]);
		close(cgi_output[1]);

		size_t i = 0;
		char ch = '\0';

		if(strcasecmp(method,"POST") == 0)
		{
			for( ; i < content_length; ++i)
			{
				recv(sock,&ch,1,0);
				write(cgi_input[1],&ch,1);
			}
		}

		while(read(cgi_output[0],&ch,1) > 0)
		{
			send(sock,&ch,1,0);
		}

		close(cgi_input[1]);
		close(cgi_output[0]);

		waitpid(pid,NULL,0);
	}
}

void echo_www(int sock,char *path, int len)
{
	int fd = open(path, O_RDONLY);

	char buf[_SIZE_] = "HTTP/1.1 200 OK \r\n\r\n";
	send(sock,buf,strlen(buf),0);

	ssize_t ret = sendfile(sock, fd, NULL, len);
	if(ret < 0)
	{
		perror("sendfile");
		return ;
	}

	close(fd);
}

void *request(void *arg)
{
	pthread_detach(pthread_self());

	int sock = (int)arg;
	char buf[_SIZE_];
	char method[_SIZE_]; //方法
	char url[_SIZE_];    
	char path[_SIZE_];
	char *query_string =  NULL;
	
	memset(buf,'\0',sizeof(buf));
	int ret  = -1;

	ret = get_line(sock,buf,sizeof(buf));
	if(ret < 0)
	{
		perror("get_line");
		return (void*)1;
	}

	printf("line : %s \n",buf);
	int i = 0;
	int j = 0;

	while((i < sizeof(method)-1) && (j < sizeof(buf)-1))
	{
		if(buf[j] == ' ')
		{
			break;
		}
		method[i] = buf[j];
		++i;
		++j;
	}
	method[i] = '\0';

	//处理空格
	while((j < sizeof(buf)-1) && isspace(buf[j]))
	{
		++j;
	}

	i = 0;
	while((i < sizeof(url)-1) && (j < sizeof(buf)-1) && (!isspace(buf[j])))
	{
		if(buf[j] == ' ')
		{
			break;
		}
		
		url[i] = buf[j];
		++i;
		++j;
	}
	url[i] = '\0';

	int cgi = 0;
	
	if(strcasecmp(method,"GET") != 0 && strcasecmp(method,"POST") != 0)
	{
		echo_error(sock,404);
		return (void*)-1;
	}

	if(strcasecmp(method,"POST") == 0)
	{
		cgi = 1;
	}

	if(strcasecmp(method,"GET") == 0)
	{
		query_string = url;
		while(*query_string != '\0'&& *query_string != '?')
		{
			query_string++;
		}
		if(*query_string == '?')
		{
			cgi = 1;
			*query_string = '\0';
			++query_string;
		}
	}

	sprintf(path,"htdoc%s",url);

	if(strcmp(url,"/") == 0)
	{
		strcat(path,"index.html");
	}

	printf("path : %s \n",path);
//	printf("url : %s \n",url);
//	printf("query_string : %s \n",query_string);

	struct stat st  ;

	if(stat(path, &st) < 0)
	{
		perror("stat");
		echo_error(sock,404);
		return (void *)-2;
	}

	if(S_ISDIR(st.st_mode)) //(st.st_mode & S_IFMT) == S_IFDIR
	{
		strcpy(path,"htdoc/index.html");	
	}
	else if ((st.st_mode & S_IXUSR) ||(st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH) )
	{
		cgi = 1;
	}
	else 
	{	}

	if(cgi)
	{
		exec(sock, path, method, query_string);
	}
	else
	{
		clear_header(sock);
		echo_www(sock,path,st.st_size);
	}

//	printf("method : %s \n",method);
//	printf("path : %s \n",path);

//	do
//	{
//		recv(sock, &ch, 1, MSG_PEEK);
//		ret = get_line(sock,buf,sizeof(buf));
//		printf("%s",buf);
//	}while(ret != -1 && strcmp(buf,"\n") != 0);
	
//	char buf[1024];
//
//	while(1)
//	{
//		memset(buf,'\0',sizeof(buf));
//
//		ssize_t size = read(sock,buf,sizeof(buf)-1);
//		if(size > 0)
//		{
//			buf[size-1] = '\0';
//			printf("client : %s\n",buf);
//
//			char buf1[] = "HTTP/1.1 200 OK\r\nContent-Length:128\r\n\r\n \
//						   <html> <head> <title>Http</title>  </head>  \
//						   <body> <h1> hello world </h1> </body> </html>";
//
//			write(sock,buf1,sizeof(buf1));
//			
//			sleep(2);
//			close(sock);
//			exit(2);
//		}
//		else if(size == 0)
//		{
//			printf("client done.... \n");
//			close(sock);
//			exit(5);
//		}
//		else
//		{
//			perror("read");
//			exit(6);
//		}
//	}

	close(sock);

	return (void *)0;
}

int main(int argc ,char *argv[])
{
	if(argc !=  3)
	{
		usage(argv[0]);
		exit(1);
	}

	int listen_sock = startup(argv[1],atoi(argv[2]));

	struct sockaddr_in peer;
	socklen_t len  = sizeof(peer);

	int done = 0;
	while(!done)
	{
		int sock = accept(listen_sock,(struct sockaddr *)&peer,&len);

		printf("create new client : [%s] -> [%d] \n" ,inet_ntoa(peer.sin_addr),ntohs(peer.sin_port));

		if(sock > 0)
		{
			pthread_t th1;
			pthread_create(&th1, NULL ,request,(void*)sock);
		}
		else
		{
			perror("accept");
			exit(7);
		}
	}

	return 0;
}


