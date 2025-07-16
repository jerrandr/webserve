#ifndef REQUETTE_HPP
#define REQUETTE_HPP

#include "Cgi.hpp"
class Pollfd;
class Cgi;

class Requette
{
    private:
        Cgi *cgi;
        Pollfd *pl;
		std::map<std::string, std::string> rq;
		std::vector<Location> lc;
        char	**envp;
		int 	lv;
        std::string body;
        void	initEnvp(std::string rt);
        Requette(Requette const & cpy);
        Requette & operator=(Requette const & cpy);
        Location findLoc();
        std::string redir_rp(std::string redir);
        std::stringstream getData(std::string filename);
    public:
        Requette(std::map<std::string, std::string> config, Client cl);
        void    rp(int socket);
        ~Requette();
};

#endif