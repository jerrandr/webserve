#ifndef REQUETTE_HPP
#define REQUETTE_HPP

#include "Cgi.hpp"
class Pollfd;
class requette
{
    private:
        Cgi exec;
        Pollfd *pl;
        std::map<std::string, std::string> cf;
        int lh;
        requette(requette const & cpy);
        requette & operator=(requette const & cpy);
    public:
        requette(std::map<std::string, std::string> config, int length , Pollfd *p);
        ~requette();
};

#endif