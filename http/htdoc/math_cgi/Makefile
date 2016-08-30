ROOT_PATH=$(shell pwd)
MATH=math_cgi

MATH_SRC=$(shell ls $(ROOT_PATH)| grep -E "*.cpp")
MATH_OBJ=$(MATH_SRC:.cpp=.o)

.PHONY:all
all:$(MATH)

$(MATH):$(MATH_OBJ)
	g++ -o $@ $^

%.o:$(ROOT_PATH)/%.cpp
	g++ -c $<

.PHONY:clean
clean:
	rm -f  math_cgi *.o
