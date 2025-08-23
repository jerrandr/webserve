NAME = webserv

SRC = main.cpp Client.cpp Server.cpp Socket.cpp utils.cpp\
	Pollfd.cpp Config.cpp Location.cpp ErrorPage.cpp \
	signal_handling/SignalHandling.cpp \
	exec_bad_request.cpp\
	NotReady.cpp\
	exec_request_timeout.cpp\
	chunked.cpp\
	request_processing.cpp\
	exec_error_server.cpp\
	directory_listing/directory_listing.cpp\
	WebServer.cpp\
	infile_parsing/config_parsing.cpp \
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
		make -C response
		${cc} -Lresponse ${FLAG} ${OBJ} -o ${NAME} -lresponse
clean :
		rm -f ${OBJ}
		make -C response clean

fclean : clean
		rm -f ${NAME}
		make -C response fclean

re : fclean all

r : re
	clear
	valgrind ./${NAME}
	
.PHONY : all clean fclean re
