/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Requette.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 12:26:36 by jerrandr          #+#    #+#             */
/*   Updated: 2025/07/28 11:03:50 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Requette.hpp"

//+++++++++++++++++++++++++CONSTRUCTO && DESTRUCTOR+++++++++++++++++++++++++

Requette::~Requette() {}

Requette::Requette(std::map<std::string, std::string> config, Client cl)
{
    rq = config;
    lv = cl.get_len_real_body();
    pl = cl.getPoll();
    this->lc = cl.getConfig().get_locs();
	body = cl.getBody();
	// std::cout << RED << "BODY: " << body << R << std::endl;
}

//+++++++++++++++++++++++++FIND LOCATION+++++++++++++++++++++++++
int	Requette::findLoc2(std::vector<std::string> UriLoc, std::vector<std::string> toFind)
{
	int nb;

	nb = 0;
	for (std::vector<std::string>::iterator j = UriLoc.begin(); j < UriLoc.end(); j++)
	{
		if (toFind[nb] == (*j) && nb < static_cast<int>(toFind.size()))
		{
			nb++;
			if (j == (UriLoc.end() - 1))
				break;
		}
	}
	return (nb);
}

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
			nb = findLoc2(tmp, toFind);
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

//+++++++++++++++++++++++++INITIALISATION ENVP+++++++++++++++++++++++++

void	Requette::initEnvp(std::string rt)
{
	std::string 		cl;
	std::string 		query;
	std::string			*tmp;
	
	envp = new char*[8];
	cl = utils.ToString(lv);
	query = "";
	tmp = new std::string[8]();
	if (rq["uri"].find("?") != std::string::npos)
		query = rq["uri"].substr(rq["uri"].find("?") + 1, rq["uri"].length() - 1);
	tmp[0] = "GATEWAY_INTERFACE=CGI/1.1";
	tmp[1] = "REQUEST_METHOD=" + rq["method"];
	if (rq["method"] == "GET")
		tmp[2] = "QUERY_STRING=" + query;
	else if (rq["method"] == "POST")
		tmp[2] = "CONTENT_LENGTH=" + cl;
	tmp[3] = "SCRIPT_FILENAME=" + rt;
    tmp[4] = "SERVER_PROTOCOL=HTTP/1.1";
    tmp[5] = "REDIRECT_STATUS=200";
	tmp[6] = "CONTENT_TYPE=application/x-www-form-urlencoded";
	for (size_t i = 0; i < 7; i++)
		envp[i] = const_cast<char*>(tmp[i].c_str());
	envp[7] = NULL;
}

//+++++++++++++++++++++++++REDIRECTION+++++++++++++++++++++++++

std::string	Requette::redir_rp2(std::string redir)
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
	}
	return (rp);
}


void	Requette::redir_rp(std::string redir, int socket)
{
	std::string	rp;

	rp = redir_rp2(redir);
	if (rp != "")
	{
		// std::cout << "rp: " << rp << std::endl;
		if ((pl->get_status(socket) & POLLOUT) && !(pl->get_status(socket) & POLLHUP))
			send(socket, rp.c_str(), rp.size(), 0);
		return ;
	}
}

//+++++++++++++++++++++++++REPONSE+++++++++++++++++++++++++

void	Requette::ifCgi(Location Loc, int socket)
{
	std::string rt = "";
	if (rq["uri"].find(Loc.get_uri()) != std::string::npos)
		rt = rq["uri"].substr(rq["uri"].find(Loc.get_uri()), rq["uri"].length());
	else
		rt = rq["uri"];
	initEnvp(rt);
	Cgi cgi(envp, lv, pl);
	cgi.MyExec(socket, body);
	return;
}

void	Requette::rp3(int socket)
{
	std::stringstream	data = utils.getData("error/405.html");
	std::string			strData = data.str();
	std::stringstream	convert;
	std::string			nbr;
	convert << strData.size();
	convert >> nbr;
	std::string rp = "HTTP/1.1 405 Method not allowed\r\nContent-Length: " + nbr + "\r\nContent-Type: text/html\r\n\r\n" + strData;
	if ((pl->get_status(socket) & POLLOUT) && !(pl->get_status(socket) & POLLHUP))
		send(socket, rp.c_str(), rp.size(), 0);
}



void	Requette::rp2(int socket)
{
	if (body == "")
	{
		int					fl;
		std::string			rt;
		std::stringstream	data;
		struct stat			file;
		std::string			nbr;
		std::string			rp;

		memset(&file, 0, sizeof(file));
		fl = 0;
		rt = "/";
		if (rq["uri"].size() > 1)
			rt = rq["uri"].substr(1, rq["uri"].length());
		else
			rt = "index.html";
		data = utils.getData(rt);
		if (data.str() == "")
		{
			data = utils.getData("error/404.html");
			fl++;
		}
		if (stat(rt.c_str(), &file) == -1)
			perror("TSY METY\n");
		nbr = utils.ToString(file.st_size);
		if (fl != 0)
			rp = "HTTP/1.1 404 not found\r\nContent-Length: " + nbr + "\r\nContent-Type: text/html\r\n\r\n" + data.str();
		else
			rp = "HTTP/1.1 200 OK\r\nContent-Length: " + nbr + "\r\nContent-Type: text/html\r\n\r\n" + data.str();
		if ((pl->get_status(socket) & POLLOUT) && !(pl->get_status(socket) & POLLHUP))
			send(socket, rp.c_str(), rp.size(), 0);
	}
	else
		rp3(socket);
}

void    Requette::rp(int socket)
{
	Location Loc = findLoc();

	std::cout << "CGI_SCRIPT: " << Loc.get_script_cgi() << std::endl; 
	if (Loc.get_redir() != "")
	{
		std::cout << "REDIRECTION\n";
		redir_rp(Loc.get_redir(), socket);
	}
	else if (Loc.get_path_cgi() != "")
	{
		std::cout << "SCRIPT_CGI\n";
		ifCgi(Loc, socket);
	}	
	else
	{
		std::cout << "NORMAL\n";
		rp2(socket);
	}
}