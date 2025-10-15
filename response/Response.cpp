/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 12:26:36 by jerrandr          #+#    #+#             */
/*   Updated: 2025/10/13 12:14:54 by jerrandr         ###   ########.fr       */
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
	qr = "";
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
	
	path = utils->getRealPathUpload(Loc.get_path_upload(), Loc);
	rp = utils->CheckError(path, Cl);
	if (rp != "")
	{
		utils->SendResponse(Cl, rp, socket);
		return ;
	}
	BodyUpload	bd(path);
	if (rq["method"] == "POST" && Loc.get_path_upload() != "")
	{
		rp = utils->CheckError(path, Cl);
		if (rp == "" && is_directory(path))
		{
			bd.UploadHandler(Cl);
			filename = Cl.getConfig().get_real_path(rq["uri"], Loc);
			rp = bd.rp_201();
			utils->SendResponse(Cl, rp, socket);
		}
	}
}

void	Response::rp2(int socket, Location &Loc)
{
	std::string	rt;
	std::string	rp;
	
	if (Cl.sd != NULL)
	{
		utils->SendResponse(Cl, Cl.sd->getData(), socket);
		return ;
	}
	if (rq["method"] == "GET" && Cl.is_dir_listing(rq["uri"]) != 1)
	{
		rt = Cl.getConfig().get_real_path(rq["uri"], Loc);
		utils->getQuery(rt);
		rp = utils->CheckError(rt, Cl);
		if (rp == "" && is_directory(rt))
		{
			if (Loc.get_index() != "")
			{
				rt = Cl.getConfig().get_real_path("/" + Loc.get_index(), Loc);
				rp = rp5(rt);
			}
			else
				rp = utils->getError(Cl, 404);
		}
		else if (rp == "")
			rp = rp5(rt);
		utils->SendResponse(Cl, rp, socket);
	}
	else
		rp3(socket, Loc);
}

void    Response::rp(int socket)
{
	Location	Loc;
	std::string	rp;

	qr = utils->getQuery(rq["uri"]);
	if (qr != "")
		rq["uri"] = rq["uri"].substr(0, rq["uri"].find(qr) - 1);
	rp = "";
	Loc = Cl.getConfig().get_location_match(rq["uri"]);
	if (rq["uri"].size() >= 2048)
	{
		rp = utils->getError(Cl, 414);
		utils->SendResponse(Cl, rp, socket);
		return ;
	}
	if (Loc.get_redir() != "")
		redir_rp(Loc.get_redir(), socket);
	else if (Check405_501(Loc, socket) || IfDelete(socket, Loc) == 1)
		return ;
	else if (ifCgi2(Loc))
		ifCgi(Loc, socket, ctType);
	else if (Cl.is_dir_listing(rq["uri"]) == 1 && rq["method"] == "GET")
			Cl.exec_dir_listing(rq["uri"]);
	else if (rq["method"] == "GET" || rq["method"] == "POST")
		rp2(socket, Loc);
}