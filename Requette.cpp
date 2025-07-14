#include "Requette.hpp"

Requette::Requette(std::map<std::string, std::string> config, int length , Pollfd *p, std::vector<Location> lc)
{
    rq = config;
    lv = length;
    pl = p;
    this->lc = lc;
}

Requette::~Requette() {} 

Location Requette::findLoc()
{
	std::vector<std::string>		toFind;
	std::vector<Location>::iterator fl = lc.end();
	int nb = 0;
	int max = 0;

	toFind = split(rq["uri"], "/");
	for (std::vector<Location>::iterator i = lc.begin(); i < lc.end(); i++)
	{
		std::vector<std::string> tmp;
		tmp = split((*i).get_uri(), "/");
		if (tmp.size() <= toFind.size())
		{			
			for (std::vector<std::string>::iterator j = tmp.begin(); j < tmp.end(); j++)
			{
				if (toFind[nb] == (*j) && nb < static_cast<int>(toFind.size()))
				{
					nb++;
					if (j == (tmp.end() - 1))
						break;
				}
			}
		}
		if (max < nb)
		{
			fl = i;
			max = nb;
		}
		nb = 0;
	}
	if (fl != lc.end())
		return (*fl);
	return (*(lc.begin()));
}

void	Requette::initEnvp(std::string rt)
{
	envp = new char*[8];
	std::stringstream 	c;
	std::string 		cl;
	std::string 		query = "";

	c << lv;
	c >> cl;
	query = rq["uri"];
	if (query.find("?") != std::string::npos)
		query = query.substr(query.find("?") + 1, query.length() - 1);
	std::string *tmp = new std::string[7]();
	tmp[0] = "GATEWAY_INTERFACE=CGI/1.1",
	tmp[1] = "REQUEST_METHOD=" + rq["method"];
	if (rt == "/")
		tmp[2] = "SCRIPT_FILENAME=www/index.html";
	else
		tmp[2] = "SCRIPT_FILENAME=" + rt;
	tmp[3] = "QUERY_STRING=" + query;
    tmp[4] = "SERVER_PROTOCOL=HTTP/1.1";
	tmp[5] = "CONTENT_LENGTH=" + cl;
    tmp[6] = "REDIRECT_STATUS=200";
	for (size_t i = 0; i < 7; i++)
		envp[i] = const_cast<char*>(tmp[i].c_str());
	envp[7] = NULL;
}

std::string Requette::redir_rp(std::string redir)
{
	std::vector<std::string> data;
	std::stringstream		convert;
	std::string				rp = "";
	int status = 0;

	data = split(redir, " ");
	convert << data[0];
	convert >> status;
	
	if (data.size() == 2)
	{
		switch (status)
		{
			case 301:
				rp = "HTTP/1.1 301 Moved Permanently\r\n";
				break;
			case 302:
				rp = "HTTP/1.1 302 Found\r\n";
				break;
			case 307:
				rp = "HTTP/1.1 307 Temporary Redirect\r\n";
				break;
			case 308:
				rp = "HTTP/1.1 308 Permanent Redirect\r\n";
				break;
			default:
				rp = "HTTP/1.1 301 Moved Permanently\r\n";
				break;
		}
		rp += "Location: " + data[1] + "\r\n\r\n";
		return (rp);
	}
	return ("");
}

std::stringstream Requette::getData(std::string filename)
{
	std::ifstream		file(filename);
	std::string			str;
	std::stringstream	data;

	std::cout << "FILENAME: " << filename << std::endl;
	if (file.fail())
		std::cout << "ERROR++++" << std::endl;
	while (getline(file, str))
		data << str;
	return (data);
}

void    Requette::rp(int socket)
{
	Location Loc = findLoc();
	
	if (Loc.get_redir() != "")
	{
		std::cout << "REDIRECTION\n";
		std::string redir = redir_rp(Loc.get_redir());
		if (redir != "")
		{
			std::cout << "REDIR: " << redir << std::endl;
			if (pl->get_status(socket) & POLLIN)
				send(socket, redir.c_str(), redir.size(), 0);
			return ;
		}
	}
	if (Loc.get_script_cgi() != "")
	{
		std::cout << "CGI\n";
		initEnvp((Loc.get_uri() + Loc.get_root()));
		Cgi cgi(envp, lv, pl);

		cgi.MyExec(socket);
		return;
	}
	else
	{
		std::cout << "NORMALE\n";
		std::stringstream	data = getData(Loc.get_root());
		std::string			strData = data.str();
		std::stringstream	convert;
		std::string			nbr;

		convert << strData.size();
		convert >> nbr;
        std::string rp = "HTTP/1.1 200 OK\r\nContent-Length: " + nbr + "\r\nContent-Type: text/html\r\n\r\n" + strData;
		if (pl->get_status(socket) & POLLIN)
			send(socket, rp.c_str(), rp.size(), 0);
	}
	
}