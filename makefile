OBJS = Rect.cpp main.cpp

COMPILER_FLAGS = -Wall -Werror -g -std=c++11

OBJ_NAME = exe

all : $(OBJS)
	g++ $(OBJS) $(COMPILER_FLAGS) -o $(OBJ_NAME)
