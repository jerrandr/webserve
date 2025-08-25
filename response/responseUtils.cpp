/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:59:37 by jerrandr          #+#    #+#             */
/*   Updated: 2025/08/25 15:01:10 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Requette.hpp"

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
			rp = utils.getData("error/404.html", pl, Cl.getFdWait(), fl);
		if (access(rt.c_str(), O_RDWR) != 0 && rp == "")
			rp = utils.getData("error/403.html",  pl, Cl.getFdWait(), fl);
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