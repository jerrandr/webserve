NAME = web

SRC = main.cpp Client.cpp Server.cpp Socket.cpp utils.cpp\
	Pollfd.cpp Config.cpp Location.cpp ErrorPage.cpp Cgi.cpp\

OBJ = ${SRC:.cpp=.o}

cc = c++

FLAG = -Wall -Wextra -Werror -std=c++98 -g

.cpp.o:
	${cc} ${FLAG} -c $< -o ${<:.cpp=.o}

all : ${NAME}

${NAME} : ${OBJ}
			${cc} ${FLAG} ${OBJ} -o ${NAME}
clean :
		rm -f ${OBJ}

fclean : clean
		rm -f ${NAME}

re : fclean all

.PHONY : all clean fclean re