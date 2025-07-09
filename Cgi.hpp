
#ifndef CGI_HPP
#define CGI_HPP

#include "Client.hpp"
#include "Socket.hpp"
#include "Server.hpp"
#include <sys/wait.h>
#include "Pollfd.hpp"
#include <iostream>

#define RED "\033[31m"
#define R "\033[0m"

class Cgi
{
    private:
		Cgi & operator=(Cgi const & cpy);
		Cgi(Cgi const & cpy);
		char **envp;
		char **argv;
		char *CgiName;
		int lv;
		std::map<std::string, std::string> rq;
		std::vector<Location> lc;
		Pollfd * pl;
		std::string findLoc();
		void	sendImg(std::string path, int socket);
		void	sendPdf(std::string path, int socket);
		void initEnvp(std::string rt);
		std::string parseUri(std::string BrutUri);
	public:
		Cgi(std::map<std::string, std::string> config, int length , Pollfd *p, std::vector<Location> l);
		void	MyExec(int fdc);
		~Cgi();	
};

#endif