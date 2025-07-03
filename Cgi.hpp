
#ifndef CGI_HPP
#define CGI_HPP

#include "Client.hpp"
#include "Socket.hpp"
#include "Server.hpp"
#include <sys/wait.h>

class Cgi
{
    private:
		Cgi & operator=(Cgi const & cpy);
		Cgi(Cgi const & cpy);
		char **envp;
		char **argv;
		char *CgiName;
		void initEnvp(std::map<std::string, std::string> config);
	public:
		Cgi(std::map<std::string, std::string> config);
		void	MyExec(int fdc);
		~Cgi();	
};

#endif