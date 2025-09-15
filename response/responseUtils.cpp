/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:59:37 by jerrandr          #+#    #+#             */
/*   Updated: 2025/09/15 14:54:07 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Requette.hpp"

void	Requette::initEnvp(std::string rt, std::string bd)
{
	std::string 		cl;
	std::string 		query;
	std::string			*tmp;
	
	envp = new char*[9];
	cl = utils->ToString(lv);
	query = "";
	tmp = new std::string[8]();
	if (rq["uri"].find("?") != std::string::npos)
		query = rq["uri"].substr(rq["uri"].find("?") + 1, rq["uri"].length() - 1);
	tmp[0] = "GATEWAY_INTERFACE=CGI/1.1";
	tmp[1] = "REQUEST_METHOD=" + rq["method"];
	tmp[2] = "SERVER_PROTOCOL=HTTP/1.1";
    tmp[3] = "REDIRECT_STATUS=200";
	tmp[4] = "CONTENT_TYPE=" + bd;
	tmp[5] = "SCRIPT_FILENAME=" + rt;

	std::cout << "Rt: " << rt << std::endl;
	if (rq["method"] == "GET")
		tmp[6] = "QUERY_STRING=" + query;
	else if (rq["method"] == "POST")
		tmp[6] = "CONTENT_LENGTH=" + cl;
	else
		tmp[6] = "";
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

int	Requette::IfDelete(int socket)
{
	std::string rt;
	std::string	rp;
	std::string	nbr;
	int			fl;
	struct stat st;

	rt = "";
	rp = "";
	fl = 0;
	if (rq["method"] == "DELETE")
	{
		if (rq["uri"].size() > 1)
			rt = rq["uri"].substr(1, rq["uri"].length());
		if (stat(rt.c_str(), &st) == -1 && rp == "")
			rp = utils->getData("error/404.html", Cl, fl);
		if (access(rt.c_str(), O_RDWR) != 0 && rp == "")
			rp = utils->getData("error/403.html", Cl, fl);
		if (rp == "")
		{
			std::remove(rt.c_str());
			rp = "HTTP/1.1 204 No Content\r\nConnection: close\r\n\r\n";
		}
		utils->SendResponse(Cl.getPoll(), rp, socket);
		return (1);
	}
	return (-1);
}

int	Requette::IfDirList(Location lt)
{
	struct stat st;

	std::cout << "uri: {" << rq["uri"] << "}\n";
	if (stat(Cl.getConfig().get_real_path(rq["uri"], lt).c_str(), &st) == -1)
		perror("stat Error");
	else
	{
		if (S_ISDIR(st.st_mode) && lt.get_directory_listing())
		{
			std::cout << "ETOOO\n";
			Cl.exec_dir_listing(rq["uri"]);
			return (1);
		}
	}
	return (0);
}

void	Requette::ifCgi(Location Loc, int socket, std::string bd)
{
	std::cout << RED << "CGI\n" << R;
	std::string rt;
	std::string	bdy;

	bdy = Cl.getBody();
	// if (Loc.get_path_cgi() == "")
	// {
	// 	rt = utils->getError("error/502.html", pl, Cl.getFdWait());
	// 	if ((pl->get_status(socket) & POLLOUT) && !(pl->get_status(socket) & POLLHUP))
	// 	{
	// 		std::cout << "RP: {" << rt << "}\n";
	// 		send(socket, rt.c_str(), rt.size(), 0);
	// 	}
	// 	return ;
	// }
	rt = Cl.getConfig().get_real_path(rq["uri"], Loc);
	initEnvp(rt, bd);
	Cgi cgi(envp, lv, Cl);
	cgi.MyExec(socket, bdy);
	return ;
}

bool	Requette::ifCgi2(Location Loc)
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