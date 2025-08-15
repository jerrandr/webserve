/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Requette.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 12:26:36 by jerrandr          #+#    #+#             */
/*   Updated: 2025/08/15 14:42:59 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Requette.hpp"
#include "Client.hpp"
//+++++++++++++++++++++++++CONSTRUCTO && DESTRUCTOR+++++++++++++++++++++++++

Requette::~Requette() {}

Requette::Requette(std::map<std::string, std::string> config, Client &cl): Cl(cl)
{
	ctType = "";
	for (std::map<std::string, std::string>::iterator i = config.begin(); i != config.end(); i++)
	{
		if ((*i).first == "Content-Type")
		{
			ctType = (*i).second;
			break ;
		}
	}
    rq = config;
    lv = cl.get_len_real_body();
    pl = cl.getPoll();
    this->lc = cl.getConfig().get_locs();
	body = cl.getBody();
	// BodyUpload bd(body);

	// bd.ParseBody();
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

void	Requette::initEnvp(std::string rt, std::string st, std::string bd)
{
	std::string 		cl;
	std::string 		query;
	std::string			*tmp;
	
	envp = new char*[9];
	cl = utils.ToString(lv);
	query = "";
	tmp = new std::string[8]();
	if (rq["uri"].find("?") != std::string::npos)
		query = rq["uri"].substr(rq["uri"].find("?") + 1, rq["uri"].length() - 1);
	tmp[0] = "GATEWAY_INTERFACE=CGI/1.1";
	tmp[1] = "REQUEST_METHOD=" + rq["method"];
	tmp[2] = "SERVER_PROTOCOL=HTTP/1.1";
    tmp[3] = "REDIRECT_STATUS=200";
	tmp[4] = "CONTENT_TYPE=" + bd;
	
	std::cout << "Rt: " << rt << std::endl;
	if (rq["method"] == "GET")
	{
		tmp[5] = "SCRIPT_FILENAME=" + rt;
		tmp[6] = "QUERY_STRING=" + query;
	}
	else if (rq["method"] == "POST")
	{
		std::cout << "++++++++++++++++\n";
		tmp[5] = "SCRIPT_FILENAME=" + st;
		tmp[6] = "CONTENT_LENGTH=" + cl;
	}
	else
	{
		tmp[5] = "";
		tmp[6] = "";
	}
	std::cout << "TMP{4} : " << tmp[4] << std::endl;
	for (size_t i = 0; i < 7; i++)
	{
		if (tmp[i] != "")
			envp[i] = const_cast<char*>(tmp[i].c_str());
		else
			envp[i] = NULL;
	}
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
		rp += "Location: " + data[1] + "\r\n\r\n"; 
	}
	return (rp);
}


void	Requette::redir_rp(std::string redir, int socket)
{
	std::string	rp;

	rp = redir_rp2(redir);
	if (rp != "")
	{
		if ((pl->get_status(socket) & POLLOUT) && !(pl->get_status(socket) & POLLHUP))
			send(socket, rp.c_str(), rp.size(), 0);
		return ;
	}
}

//+++++++++++++++++++++++++REPONSE+++++++++++++++++++++++++

void	Requette::ifCgi(Location Loc, int socket, std::string bd)
{
	std::cout << RED << "CGI\n" << R;
	std::string rt = "";
	if (rq["uri"].find(Loc.get_uri()) != std::string::npos)
		rt = rq["uri"].substr(Loc.get_uri().size(), rq["uri"].length());
	else
		rt = rq["uri"];
	std::cout << "BD: " << bd << std::endl;
	initEnvp(rt, Loc.get_script_cgi(), bd);
	Cgi cgi(envp, lv, Cl);
	cgi.MyExec(socket, body);
	return ;
}

void	Requette::rp3(int socket)
{
	std::string	rp;

	if (body.find("filename") != std::string::npos)
	{
		BodyUpload bd(body);
		bd.ParseBody();
	}
	rp = utils.getError("error/405.html", pl, Cl.getFdWait());
	if ((pl->get_status(socket) & POLLOUT) && !(pl->get_status(socket) & POLLHUP))
		send(socket, rp.c_str(), rp.size(), 0);
}


void	Requette::rp2(int socket)
{
	std::string	ext;

	std::cout << RED << "NORMALE\n" << R;
	if (body == "")
	{
		std::string			rt;
		std::stringstream	data;
		struct stat			file;
		std::string			nbr;
		std::string			rp;

		memset(&file, 0, sizeof(file));
		rt = "/";
		if (rq["uri"].size() > 1)
			rt = rq["uri"].substr(1, rq["uri"].length());
		std::cout << RED << "FILE: " << rt << R << std::endl;
		if (stat(rt.c_str(), &file) == -1)
		{
			rp = utils.getError("error/404.html", pl, Cl.getFdWait());
			std::cout << "RP: {" << rp << "}\n";
		}
		else
		{
			ext = utils.getExt(rt);
			data << utils.getData(rt, pl, Cl.getFdWait());
			nbr = utils.ToString(data.str().size());
			rp = "HTTP/1.1 200 OK\r\nContent-Length: " + nbr + "\r\nContent-Type: " + Cl.getConfig().get_mime(ext) + "\r\n\r\n" + data.str();
		}
		if ((pl->get_status(socket) & POLLOUT) && !(pl->get_status(socket) & POLLHUP))
			send(socket, rp.c_str(), rp.size(), 0);
	}
	else
		rp3(socket);
}

int	Requette::IfDelete(int socket)
{
	std::string rt;
	std::string	rp;
	std::string	nbr;
	struct stat st;

	rt = "";
	rp = "";

	if (rq["method"] == "DELETE")
	{
		if (rq["uri"].size() > 1)
			rt = rq["uri"].substr(1, rq["uri"].length());
		if (stat(rt.c_str(), &st) == -1 && rp == "")
			rp = utils.getData("error/404.html", pl, Cl.getFdWait());
		if (access(rt.c_str(), O_RDWR) != 0 && rp == "")
			rp = utils.getData("error/403.html",  pl, Cl.getFdWait());
		if (rp == "")
		{
			std::remove(rt.c_str());
			rp = "HTTP/1.1 204 No Content\r\nConnection: close\r\n\r\n";
		}
		if ((pl->get_status(socket) & POLLOUT) && !(pl->get_status(socket) & POLLHUP))
			send(socket, rp.c_str(), rp.size(), 0);
		return (1);
	}
	return (-1);
}

void    Requette::rp(int socket)
{
	Location	Loc;
	std::string rp;

	Loc = findLoc();
	if (rq["uri"].find("favicon") != std::string::npos)
		return ;
	std::cout << "METHOD: " << rq["method"] << std::endl;
	if (Loc.get_meth().find(rq["method"]) == std::string::npos)
	{
		rp = utils.getError("error/405.html", pl, Cl.getFdWait());
		if ((pl->get_status(socket) & POLLOUT) && !(pl->get_status(socket) & POLLHUP))
			send(socket, rp.c_str(), rp.size(), 0);
		return ;
	}
	if (IfDelete(socket) == 1)
		return ;
	if (Loc.get_redir() != "")
		redir_rp(Loc.get_redir(), socket);
	else if (Loc.get_path_cgi() != "")
		ifCgi(Loc, socket, ctType);
	else
		rp2(socket);
}