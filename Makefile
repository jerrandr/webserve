
C++ = c++ -Wall -Wextra -Werror

SRC = main.cpp

NAME = socket

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME):$(OBJ)
	$(C++) -o $(NAME) $(OBJ) 

clean:
	rm -rf $(OBJ)

fclean:clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all re fclean clean
	