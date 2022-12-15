NAME	= a.out
CC		= c++
ASAN	= -fsanitize=undefined,address,leak
FLAGS	= -g -Wall -std=c++17
SOURCE	= main.cpp
HEADER	= Matrix.hpp
OBJECTS	= $(patsubst %,obj/%,$(SOURCE:.cpp=.o))
INCLUDE	= -I .
TMP		= vgcore.* *.dSYM obj

all: $(NAME)

$(NAME): $(OBJECTS) $(HEADER)
	$(CC) $(FLAGS) $(INCLUDE) $(OBJECTS) -o $@

obj/%.o: %.cpp $(HEADER)
	mkdir -p obj
	$(CC) $(FLAGS) -c $< $(INCLUDE) -o $@

clean:
	rm -rf $(NAME) $(TMP)

re: clean all

.PHONY = all clean re