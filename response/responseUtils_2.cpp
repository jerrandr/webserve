/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseUtils_2.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 10:28:44 by jerrandr          #+#    #+#             */
/*   Updated: 2025/10/04 11:14:38 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

bool	Response::ifCgi2(Location Loc)
{
	std::string					rlp;
	Config						cfg;
	std::string					uriExt;
	std::vector<std::string>	ext;

	cfg = Cl.getConfig();
	rlp = cfg.get_real_path(rq["uri"], Loc);
	ext = split(Loc.get_extension_cgi(), " ");
	uriExt = utils->getExt(rlp);
	if (is_directory(rlp))
		return (false);
	if (std::find(ext.begin(), ext.end(), uriExt) == ext.end())
		return (false);
	return (true);
}

bool	Response::Check405_501(Location Loc, int socket)
{
	std::string	mth;
	std::string	rp;

	rp = "";
	mth = "GET POST DELETE";
	if (Loc.get_meth().find(rq["method"]) == std::string::npos
		|| Loc.get_meth() != mth)
	{
		if (mth.find(rq["method"]) == std::string::npos && Loc.get_meth().find(rq["method"]) != std::string::npos)
			rp = utils->getError(Cl, 501);
		else
			rp = utils->getError(Cl, 405);
		utils->SendResponse(Cl.getPoll(), rp, socket);
		return (true);
	}
	return (false);
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