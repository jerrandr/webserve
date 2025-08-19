NAME = webserv

SRC = main.cpp Client.cpp Server.cpp Socket.cpp utils.cpp\
	Pollfd.cpp Config.cpp Location.cpp ErrorPage.cpp Cgi.cpp\
	signal_handling/SignalHandling.cpp Requette.cpp\
	exec_bad_request.cpp\
	NotReady.cpp\
	exec_request_timeout.cpp\
	chunked.cpp\
	request_processing.cpp\
	exec_error_server.cpp\
	ExecUtils.cpp\
	directory_listing/directory_listing.cpp\
	WebServer.cpp\
	BodyUpload.cpp infile_parsing/config_parsing.cpp \
	infile_parsing/address_checking.cpp infile_parsing/port_checking.cpp \
	infile_parsing/paths_handling.cpp infile_parsing/location_parsing.cpp \
	infile_parsing/location_check.cpp infile_parsing/utils_check.cpp

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

r : re
	clear
	valgrind ./${NAME}
	
.PHONY : all clean fclean re
