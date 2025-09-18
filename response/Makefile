
NAME = libresponse.a

SRC =  ExecUtils.cpp BodyUpload.cpp Response.cpp Cgi.cpp \
		responseUtils.cpp redirect.cpp findLoc.cpp \
		getData.cpp 

OBJ = ${SRC:.cpp=.o}

cc = c++

FLAG = -Wall -Wextra -Werror -std=c++98 -g

.cpp.o:
	${cc} ${FLAG} -c $< -o ${<:.cpp=.o}

all : ${NAME}

${NAME} : ${OBJ}
	ar rcs ${NAME} $^

clean :
		rm -f ${OBJ}

fclean : clean
		rm -f ${NAME}

re : fclean all
	
.PHONY : all clean fclean re
