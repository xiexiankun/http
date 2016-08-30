
SQL_PATH=/home/Admin/git/project/http/mysql_connect

FIND_INCLUDE=-I/home/Admin/git/project/http/lib/mysql_lib/include -I$(SQL_PATH) 
FIND_LIB=-L/home/Admin/git/project/http/lib/mysql_lib/lib   -lmysqlclient

SELECT=select_cgi

SELECT_SRC=$(shell ls | grep -E "*.cpp")
SELECT_SRC+=$(shell ls $(SQL_PATH) | grep -E "*.cpp")
SELECT_OBJ=$(SELECT_SRC:.cpp=.o)

.PHONY:all
all:$(SELECT)

$(SELECT): $(SELECT_OBJ)
	g++ -o $@ $^  $(FIND_LIB)

%.o:./%.cpp
	g++ -c $<  $(FIND_INCLUDE) 

%.o:$(SQL_PATH)/%.cpp
	g++ -c $<  $(FIND_INCLUDE) 

.PHONY:clean
clean:
	rm -f  select_cgi *.o


