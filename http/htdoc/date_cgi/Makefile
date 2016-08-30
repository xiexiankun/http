DATE_PATH=$(shell pwd)

DATE=date_cgi

DATE_SRC=$(shell ls $(DATE_PATH) | grep -E "*.cpp")
DATE_OBJ=$(DATE_SRC:.cpp=.o)

$(DATE):$(DATE_OBJ)
	g++ -o $@ $^

%.o:$(DATE_PATH)/%.cpp
	g++ -c $<

.PHONY:clean
clean:
	rm -f date_cgi *.o
	
