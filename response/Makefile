
NAME = libresponse.a

SRC =  ExecUtils.cpp BodyUpload.cpp Response.cpp Cgi.cpp \
		responseUtils.cpp redirect.cpp findLoc.cpp \
		getData.cpp responseUtils_2.cpp Send.cpp

OBJ = ${SRC:.cpp=.o}

cc = c++

FLAG = -Wall -Wextra -Werror -std=c++98

%.o: %.cpp
	${cc} ${FLAG} -c $< -o $@

all : ${NAME}

${NAME} : ${OBJ}
	ar rcs ${NAME} $^

clean :
		rm -f ${OBJ}

fclean : clean
		rm -f ${NAME}

re : fclean all
	
.PHONY : all clean fclean re
