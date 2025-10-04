/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseUtils_2.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 10:28:44 by jerrandr          #+#    #+#             */
/*   Updated: 2025/10/04 11:19:06 by jerrandr         ###   ########.fr       */
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
	std::vector<std::string> list_allowed;
	std::vector<std::string> list_implemented;

	rp = "";
	mth = "GET POST DELETE";
	list_allowed = split(Loc.get_meth()," ");
	list_implemented = split(mth," ");
	if (std::find(list_allowed.begin(),list_allowed.end(),rq["method"]) == list_allowed.end())
	{
		rp = utils->getError(Cl, 405);
		utils->SendResponse(Cl.getPoll(), rp, socket);
		return true;
	}
	else if (std::find(list_allowed.begin(),list_allowed.end(),rq["method"]) != list_allowed.end()
		&& std::find(list_implemented.begin(),list_implemented.end(),rq["method"]) == list_implemented.end())
	{	rp = utils->getError(Cl, 501);
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