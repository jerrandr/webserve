/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:59:37 by jerrandr          #+#    #+#             */
/*   Updated: 2025/09/20 13:51:47 by jerrandr         ###   ########.fr       */
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
	if (bd != "")
		envStock.push_back("CONTENT_TYPE=" + bd);
	std::cout << "Rt: " << rt << std::endl;
	if (rq["method"] == "GET" && query != "")
		envStock.push_back("QUERY_STRING=" + query);
	else if (rq["method"] == "POST" && cl != "")
		envStock.push_back("CONTENT_LENGTH=" + cl);
	envp = new char*[envStock.size() + 1];
	for (std::vector<std::string>::iterator i = envStock.begin(); i < envStock.end(); i++)
	{
		envp[nb] = const_cast<char*>((*i).c_str());
		nb++;	
	}
	envp[nb] = NULL;
}

void	Response::Delete(std::string path)
{
	if (is_directory(path))
	{
		DIR				*dir = opendir(path.c_str());
		struct dirent	*next;
		
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
				Delete(fullpath);
			else
			{
				std::cout << RED << "{" << fullpath << "}\n";
				std::remove(fullpath.c_str());
			}
			next = readdir(dir);
		}
		std::remove(path.c_str());
		closedir(dir);
	}
	else
		std::remove(path.c_str());
}

int	Response::IfDelete(int socket)
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
			rp = utils->getError(Cl, 405);
		else if (access(rt.c_str(), O_RDWR) != 0 && rp == "")
			rp = utils->getError(Cl, 403);
		if (rp == "")
		{
			std::cout << "DELETE {" << rt << "}\n";
			Delete(rt);
			rp = "HTTP/1.1 204 No Content\r\nConnection: close\r\n\r\n";
		}
		utils->SendResponse(Cl.getPoll(), rp, socket);
		return (1);
	}
	return (-1);
}

int	Response::IfDirList(Location lt)
{
	struct stat st;

	std::cout << "uri: {" << rq["uri"] << "}\n";
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

void	Response::ifCgi(Location Loc, int socket, std::string bd)
{
	std::cout << RED << "CGI\n" << R;
	std::string rt;
	std::string	bdy;

	bdy = Cl.getBody();
	rt = Cl.getConfig().get_real_path(rq["uri"], Loc);
	initEnvp(rt, bd);
	Cgi cgi(envp, lv, Cl);
	cgi.MyExec(socket, bdy);
	return ;
}

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