/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:59:37 by jerrandr          #+#    #+#             */
/*   Updated: 2025/10/13 10:19:19 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void	Response::initEnvp(std::string rt, std::string bd)
{
	std::string	cl;
	std::string	query;
	int			nb;

	nb = 0;
	cl = utils->ToString(lv);
	query = "";
	if (rq["uri"].find("?") != std::string::npos)
		query = rq["uri"].substr(rq["uri"].find("?") + 1, rq["uri"].length() - 1);
	envStock.push_back("GATEWAY_INTERFACE=CGI/1.1");
	envStock.push_back("REQUEST_METHOD=" + rq["method"]);
	envStock.push_back("SERVER_PROTOCOL=HTTP/1.1");
    envStock.push_back("REDIRECT_STATUS=200");
	envStock.push_back("SCRIPT_FILENAME=" + rt);
	if (bd != "" && rq["method"] == "POST")
		envStock.push_back("CONTENT_TYPE=" + bd);
	if (rq["method"] == "GET" && query != "")
		envStock.push_back("QUERY_STRING=" + query);
	if (rq["method"] == "POST" && cl != "")
		envStock.push_back("CONTENT_LENGTH=" + cl);
	envp = new char*[envStock.size() + 1];
	for (std::vector<std::string>::iterator i = envStock.begin(); i < envStock.end(); i++)
	{
		envp[nb] = const_cast<char*>((*i).c_str());
		nb++;	
	}
	envp[nb] = NULL;
}

void	Response::Delete(std::string path, std::string &rp)
{
	if (rp != "")
		return ;
	if (is_directory(path))
	{
		DIR				*dir = opendir(path.c_str());
		struct dirent	*next;
		if (dir == NULL)
		{
			rp = error_403;
			return ;
		}
		next = readdir(dir);
		while (next != NULL)
		{
			std::string fullpath = path + "/" + next->d_name; 
			std::string	test = next->d_name;
			if (test == "." || test == "..")
			{
				next = readdir(dir);
				continue ;
			}
			if (is_directory(fullpath))
			{
				Delete(fullpath, rp);
				if (rp != "")
					break;
			}
			else
			{
				if ((access(fullpath.c_str(), W_OK) != 0 || std::remove(fullpath.c_str()) != 0) && rp == "")
					rp = error_403;
			}
			next = readdir(dir);
		}
		if ((access(path.c_str(), W_OK) != 0 || std::remove(path.c_str()) != 0) && rp == "")
			rp = error_403;
		closedir(dir);
	}
	else
	{
		if ((access(path.c_str(), W_OK) != 0 || std::remove(path.c_str()) != 0) && rp == "")
			rp = error_403;
	}
}

int	Response::IfDelete(int socket, Location loc)
{
	std::string rt;
	std::string	rp;
	std::string	nbr;
	struct stat st;
	Config		cf;

	cf = Cl.getConfig();
	rp = "";
	rt = cf.get_real_path(rq["uri"], loc);
	if (rq["method"] == "DELETE")
	{
		error_403 = utils->getError(Cl, 403);
		if (stat(rt.c_str(), &st) == -1 && rp == "")
			rp = utils->getError(Cl, 404);
		else if (access(rt.c_str(), W_OK) != 0 && rp == "")
			rp = error_403;
		if (rp == "")
		{
			Delete(rt, rp);
			if (rp == "")
				rp = "HTTP/1.1 204 No Content\r\nConnection: close\r\n\r\n";
		}
		utils->SendResponse(Cl, rp, socket);
		return (1);
	}
	return (-1);
}

int	Response::IfDirList(Location lt)
{
	struct stat st;

	if (stat(Cl.getConfig().get_real_path(rq["uri"], lt).c_str(), &st) == -1)
		perror("stat Error");
	else
	{
		if (S_ISDIR(st.st_mode) && lt.get_directory_listing())
		{
			Cl.exec_dir_listing(rq["uri"]);
			return (1);
		}
	}
	return (0);
}

bool	Response::Check_502(std::string str)
{
	struct stat st;
	bool		res;

	res = false;
	if (stat(str.c_str(), &st) == -1)
		res = true;
	else if (!(st.st_mode & S_IXUSR))
		res = true;
	if (access(str.c_str(), F_OK) == -1)
		res = true;
	else if (access(str.c_str(), X_OK) == -1)
		res = true;
	return (res);
}

void	Response::ifCgi(Location Loc, int socket, std::string bd)
{
	std::string rt;
	std::string	bdy;
	std::string	pth;
	std::string	rp;

	pth = Loc.get_path_cgi();
	bdy = Cl.getBody();
	rt = Cl.getConfig().get_real_path(rq["uri"], Loc);

	if (Check_502(pth))
	{
		rp = utils->getError(Cl, 502);
		utils->SendResponse(Cl, rp, socket);
		return;
	}
	initEnvp(rt, bd);
	Cgi cgi(envp, lv, Cl, Loc.get_path_cgi());
	cgi.MyExec(socket, bdy);
	return ;
}
