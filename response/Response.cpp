/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 12:26:36 by jerrandr          #+#    #+#             */
/*   Updated: 2025/09/25 09:57:37 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Response.hpp"
//+++++++++++++++++++++++++CONSTRUCTO && DESTRUCTOR+++++++++++++++++++++++++

Response::~Response()
{
	delete utils;
}

Response::Response(std::map<std::string, std::string> config, Client &cl): Cl(cl)
{
	Config	cf;
	
	cf = Cl.getConfig();
	utils = new ExecUtils();
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
}	

//+++++++++++++++++++++++++REPONSE+++++++++++++++++++++++++

void	Response::rp3(int socket, Location Loc)
{
	std::string	filename;
	std::string	rp;
	std::string	path;

	path = Loc.get_root() + Loc.get_path_upload();
	BodyUpload	bd(path);
	if (rq["method"] == "POST" && Loc.get_path_upload() != "")
	{
		rp = utils->CheckError(path, Cl);
		if (rp == "" && is_directory(path))
		{
			bd.UploadHandler(Cl);
			filename = Cl.getConfig().get_real_path(rq["uri"], Loc);
			rp = rp5(filename);
		}
	}
	utils->SendResponse(Cl.getPoll(), rp, socket);
}

std::string	Response::rp5(std::string	rt)
{
	std::stringstream	data;
	std::string	ext;
	std::string	nbr;
	std::string	rp;
	
	ext = ".html";
	data << utils->CheckError(rt, Cl);
	if (data.str() == "")
	{
		ext = utils->getExt(rt);
		data.clear();
		data << utils->getData(rt, Cl);
	}
	else
	{
		rp = data.str();
		return (rp);
	}
	nbr = utils->ToString(data.str().size());
	rp = "HTTP/1.1 200 OK\r\nContent-Length: " + nbr + "\r\nContent-Type: " + Cl.getConfig().get_mime(ext) + "\r\n\r\n" + data.str();
	return (rp);
}

void	Response::rp2(int socket, Location &Loc)
{
	std::string	rt;
	std::string	rp;
	
	if (rq["method"] == "GET")
	{
		rt = Cl.getConfig().get_real_path(rq["uri"], Loc);
		rp = utils->CheckError(rt, Cl);
		if (rp == "" && is_directory(rt) && Loc.get_index() != "")
		{
			rt = Cl.getConfig().get_real_path("/" + Loc.get_index(), Loc);
			rp = rp5(rt);
			std::cout << "RP5 {" << rp << "}\n";
		}
		else if (rp == "")
			rp = rp5(rt);
		utils->SendResponse(Cl.getPoll(), rp, socket);
	}
	else
		rp3(socket, Loc);
}

void    Response::rp(int socket)
{
	Location	Loc;
	std::string	rp;
	std::string	mth;

	mth = "GET POST DELETE";
	Loc = findLoc();	
	if (rq["uri"].size() >= 2048)
	{
		rp = utils->getError(Cl, 405); 
		utils->SendResponse(Cl.getPoll(), rp, socket);
		return ;
	}
	else if (IfDelete(socket, Loc) == 1)
		return ;
	if (Loc.get_redir() != "" && rq["method"] == "GET")
		redir_rp(Loc.get_redir(), socket);
	else if (Loc.get_meth().find(rq["method"]) == std::string::npos)
	{
		if (mth.find(rq["method"]) == std::string::npos)
			rp = utils->getError(Cl, 501);
		else
			rp = utils->getError(Cl, 405);
		utils->SendResponse(Cl.getPoll(), rp, socket);
		return ;
	}
	else if (ifCgi2(Loc))
		ifCgi(Loc, socket, ctType);
	else if (Cl.is_dir_listing(rq["uri"]) == 1)
		Cl.exec_dir_listing(rq["uri"]);
	else
	{
		std::cout << "RP2\n";
		rp2(socket, Loc);
	}
}