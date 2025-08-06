NAME = web

SRC = main.cpp Client.cpp Server.cpp Socket.cpp utils.cpp\
	Pollfd.cpp Config.cpp Location.cpp ErrorPage.cpp Cgi.cpp\
	signal_handling/SignalHandling.cpp Requette.cpp\
	ExecUtils.cpp\
	directory_listing/directory_listing.cpp\
	WebServer.cpp\
	BodyUpload.cpp config_parsing.cpp\

OBJ = ${SRC:.cpp=.o}

cc = c++

FLAG = -Wall -Wextra -Werror -std=c++11 -g

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

r : re
	clear
	valgrind ./${NAME}
	
.PHONY : all clean fclean re