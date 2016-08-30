
ROOT_PATH=$(shell pwd)
MATH_PATH=$(ROOT_PATH)/htdoc/math_cgi
INSERT_PATH=$(ROOT_PATH)/htdoc/insert_cgi
SELECT_PATH=$(ROOT_PATH)/htdoc/select_cgi
DATE_PATH=$(ROOT_PATH)/htdoc/date_cgi
SQL_API_PATH=$(ROOT_PATH)/mysql_connect

MATH=math_cgi
INSERT=insert_cgi
SELECT=select_cgi
DATE=date_cgi

MATH_SRC=$(shell ls $(MATH_PATH) | grep -E "*.cpp")
MATH_OBJ=$(MATH_SRC:.cpp=.o)

DATE_SRC=$(shell ls $(DATE_PATH) | grep -E "*.cpp")
DATE_OBJ=$(DATE_SRC:.cpp=.o)

INSERT_SRC=$(shell ls $(INSERT_PATH) | grep -E "*.cpp")
INSERT_SRC+=$(shell ls $(SQL_API_PATH) | grep -E "*.cpp")
INSERT_OBJ=$(INSERT_SRC:.cpp=.o)

SELECT_SRC=$(shell ls $(SELECT_PATH) | grep -E "*.cpp")
SELECT_SRC+=$(shell ls $(SQL_API_PATH) | grep -E "*.cpp")
SELECT_OBJ=$(SELECT_SRC:.cpp=.o)

FIND_INCLUDE=-I$(SQL_API_PATH) -I$(ROOT_PATH)/lib/mysql_lib/include  
FIND_LIB=-L$(ROOT_PATH)/lib/mysql_lib/lib  -lmysqlclient

BIN=httpd
SRC=httpd.c
CC=gcc

OBJ=$(SRC:.c=.o)

LDFLAGS=-lpthread  

.PHONY:all
all:$(BIN) $(MATH) $(INSERT) $(SELECT) $(DATE)

$(BIN):$(OBJ)
	@$(CC) -o $@  $^ $(LDFLAGS)
	@echo "LINKing  [$^] to [$@]...done "

$(MATH):$(MATH_OBJ)
	@g++ -o  $@ $^  
	@echo "LINKing  [$^] to [$@]...done "

$(INSERT):$(INSERT_OBJ)
	@g++ -o $@  $^   $(FIND_LIB)
	@echo "LINKing  [$^] to [$@]...done "

$(SELECT):$(SELECT_OBJ)
	@g++ -o $@  $^   $(FIND_LIB)
	@echo "LINKing  [$^] to [$@]...done "

$(DATE):$(DATE_OBJ)
	@g++ -o $@  $^  
	@echo "LINKing  [$^] to [$@]...done "

%.o:%.c
	@$(CC) -c  $<
	@echo "Compling [$<] to [$@]...done"

%.o:$(MATH_PATH)/%.cpp
	@g++ -c  $<  
	@echo "Compling [$<] to [$@]...done"

%.o:$(DATE_PATH)/%.cpp
	@g++ -c  $<  
	@echo "Compling [$<] to [$@]...done"

%.o:$(INSERT_PATH)/%.cpp
	@g++ -c  $<   $(FIND_INCLUDE)
	@echo "Compling [$<] to [$@]...done"

%.o:$(SELECT_PATH)/%.cpp
	@g++ -c  $<  $(FIND_INCLUDE) 
	@echo "Compling [$<] to [$@]...done"

%.o:$(SQL_API_PATH)/%.cpp
	@g++ -c  $<  $(FIND_INCLUDE)
	@echo "Compling [$<] to [$@]...done"

.PHONY:clean
clean:
	rm -f $(BIN)  $(MATH) $(INSERT)  $(SELECT) $(DATE) *.o 
	rm -rf output/

.PHONY:output
output: $(BIN)
	@rm  -rf *.o
	@mkdir -p 			output/http
	@cp -rf conf 		output/http
	@cp -rf $(BIN) 		output/
	@cp -rf start.sh  	output/
	@mkdir -p 			output/htdoc
	@mkdir -p 			output/htdoc/cgi

	@cp -rf $(ROOT_PATH)/htdoc/index.html   output/htdoc
	@cp -rf $(ROOT_PATH)/htdoc/favicon.ico  output/htdoc/favicon.ico
	@cp -rf $(ROOT_PATH)/math_cgi     		output/htdoc/cgi
	@cp -rf $(ROOT_PATH)/insert_cgi    		output/htdoc/cgi
	@cp -rf $(ROOT_PATH)/select_cgi    		output/htdoc/cgi
	@cp -rf $(ROOT_PATH)/date_cgi    		output/htdoc/cgi
	@cp -rf $(ROOT_PATH)/htdoc/images  		output/htdoc/images
	
.PHONY:debug
debug:
	@echo $(OBJ)
	@echo $(MATH_PATH)
	@echo $(MATH_OBJ)
	@echo $(INSERT_OBJ)
	@echo $(SQL_API_PATH)
	@echo $(BIGDATA_PATH)


