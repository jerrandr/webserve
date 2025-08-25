/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Requette.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 12:26:36 by jerrandr          #+#    #+#             */
/*   Updated: 2025/08/25 15:12:37 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Requette.hpp"
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
}	

//+++++++++++++++++++++++++REPONSE+++++++++++++++++++++++++

void	Requette::ifCgi(Location Loc, int socket, std::string bd)
{
	std::cout << RED << "CGI\n" << R;
	std::string rt;
	
	rt = Cl.getConfig().get_real_path(rq["uri"], Loc);
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


void	Requette::rp2(int socket, Location Loc)
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
		int					fl;

		fl = 0;
		memset(&file, 0, sizeof(file));
		rt = Cl.getConfig().get_real_path(rq["uri"], Loc);
		std::cout << RED << "FILE: " << rt << R << std::endl;
		if (stat(rt.c_str(), &file) == -1)
		{
			rp = utils.getError("error/404.html", pl, Cl.getFdWait());
			fl = -1;
		}
		else if (is_directory(rt) && Loc.get_index() != "")
			rt = Cl.getConfig().get_real_path("/" + Loc.get_index(), Loc);
		data << utils.getData(rt, pl, Cl.getFdWait(), fl);
		if (fl == -1)
			ext = ".html";
		else
			ext = utils.getExt(rt);
		nbr = utils.ToString(data.str().size());
		rp = "HTTP/1.1 200 OK\r\nContent-Length: " + nbr + "\r\nContent-Type: " + Cl.getConfig().get_mime(ext) + "\r\n\r\n" + data.str();
		if ((pl->get_status(socket) & POLLOUT) && !(pl->get_status(socket) & POLLHUP))
		{
			std::cout << "RP: {" << rp << "}\n";
			send(socket, rp.c_str(), rp.size(), 0);
		}
	}
	else
		rp3(socket);
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
	std::cout << "path: " << Loc.get_path_cgi() << std::endl;
	if (IfDelete(socket) == 1)
		return ;
	std::cout << "redir: {" << Loc.get_redir() << "}\n";
	if (Loc.get_redir() != "/")
		redir_rp(Loc.get_redir(), socket);
	else if (Loc.get_path_cgi() != "")
		ifCgi(Loc, socket, ctType);
	else
		rp2(socket, Loc);
}