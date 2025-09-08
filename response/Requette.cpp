/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Requette.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 12:26:36 by jerrandr          #+#    #+#             */
/*   Updated: 2025/09/08 10:30:36 by jerrandr         ###   ########.fr       */
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

void	Requette::rp3(int socket, Location Loc)
{
	std::string	filename;
	std::string	rp;

	BodyUpload	bd(body, Loc.get_path_upload());
	if (rq["method"] == "POST" && Loc.get_path_upload() != "")
	{
		bd.ParseBody(Cl);
		filename = Cl.getConfig().get_real_path(rq["uri"], Loc);
		rp = rp5(filename);
		std::cout << "rp = {" << rp << "}\n";
	}
	if ((pl->get_status(socket) & POLLOUT) && !(pl->get_status(socket) & POLLHUP))
		send(socket, rp.c_str(), rp.size(), 0);
}

std::string	Requette::rp4(std::string	rp)
{
	std::string	res;

	res = "";
	if (access(rp.c_str(), F_OK) == -1)
		res = utils.getError("error/404.html", pl, Cl.getFdWait());
	else if (access(rp.c_str(), R_OK) == -1)
		res = utils.getError("error/403.html", pl, Cl.getFdWait());
	return (res);	
}

std::string	Requette::rp5(std::string	rt)
{
	int					fl;
	std::stringstream	data;
	std::string			ext;
	std::string			nbr;
	std::string			rp;

	fl = 0;
	data << utils.getData(rt, pl, Cl.getFdWait(), fl);
	if (fl == -1)
		ext = ".html";
	else
		ext = utils.getExt(rt);
	nbr = utils.ToString(data.str().size());
	rp = "HTTP/1.1 200 OK\r\nContent-Length: " + nbr + "\r\nContent-Type: " + Cl.getConfig().get_mime(ext) + "\r\n\r\n" + data.str();
	return (rp);
}

void	Requette::rp2(int socket, Location &Loc)
{
	std::cout << RED << "NORMALE\n" << R;
	std::string	rt;
	std::string	rp;
	
	if (rq["method"] == "GET")
	{
		rt = Cl.getConfig().get_real_path(rq["uri"], Loc);
		std::cout << RED << "FILE: " << rt << R << std::endl;
		rp = rp4(rt);
		std::cout << "RP {" << rp << "}\n";

		if (rp == "" && is_directory(rt) && Loc.get_index() != "")
		{
			rt = Cl.getConfig().get_real_path("/" + Loc.get_index(), Loc);
			rp = rp5(rt);
		}
		else if (rp == "")
			rp = rp5(rt);
		if ((pl->get_status(socket) & POLLOUT) && !(pl->get_status(socket) & POLLHUP))
			send(socket, rp.c_str(), rp.size(), 0);
	}
	else
		rp3(socket, Loc);
}

void    Requette::rp(int socket)
{
	Location	Loc;
	std::string rp;

	Loc = findLoc();

	std::cout << RED << "LOC : {" << Loc.get_uri() << "}\n" << R;
	std::cout << "METHOD: " << rq["method"] << std::endl;
	if (Loc.get_redir() != "")
		redir_rp(Loc.get_redir(), socket);
	else if (Loc.get_meth().find(rq["method"]) == std::string::npos)
	{
		rp = utils.getError("error/405.html", pl, Cl.getFdWait());
		if ((pl->get_status(socket) & POLLOUT) && !(pl->get_status(socket) & POLLHUP))
			send(socket, rp.c_str(), rp.size(), 0);
		return ;
	}
	else if (IfDelete(socket) == 1)
		return ;
	else if (ifCgi2(Loc))
		ifCgi(Loc, socket, ctType);
	else
		rp2(socket, Loc);
}