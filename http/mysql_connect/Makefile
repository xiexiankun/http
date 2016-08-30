WORK_PATH=$(shell pwd)
  
BIN=sql_api
CC=g++
INCLUDE=-I$(WORK_PATH)/../lib/mysql_lib/include 
LIB_PATH=-L$(WORK_PATH)/../lib/mysql_lib/lib  
SRC=sql_api.cpp

$(BIN):$(SRC)
	$(CC) -o $@ $^ $(INCLUDE) $(LIB_PATH)  -lmysqlclient  

.PHONY:clean
clean:
	rm -f ${BIN}
