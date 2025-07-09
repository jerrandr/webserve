#include "requette.hpp"

requette::requette(std::map<std::string, std::string> config, int length , Pollfd *p)
{
    cf = config;
    lh = length;
    pl = p;
}

requette::~requette() {} 