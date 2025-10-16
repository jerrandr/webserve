/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getData.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 18:04:38 by jerrandr          #+#    #+#             */
/*   Updated: 2025/10/07 20:57:50 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecUtils.hpp"

std::string ExecUtils::getData(std::string filename, Client & cl)
{
	std::string			data;
	int					fd;

	data = "";
	fd = fd_is_ready(filename, cl.getPoll(), cl.getFdWait());
	if (fd == -1)
		throw NotReady();
	data = getData(fd);
	cl.st = false;
	return (data);
}

std::vector<std::pair<int, std::string> >	ExecUtils::get_403_404(std::string str)
{
	std::vector<std::pair<int, std::string> >	res;
	std::string						data;
	int								st;

	st = 0;
	data = "";
	if (access(str.c_str(), F_OK) == -1)
	{
		st = 404;
		data = "<h1> 404 error </h1>";
	}
	else if (access(str.c_str(), R_OK) == -1)
	{
		st = 403;
		data = "<h1> 403 error </h1>";
	}
	res.push_back(std::pair<int, std::string >(st, data));
	return (res);
}


std::string	ExecUtils::getData(int fd)
{
	std::string	res;
	char		buff[1048576];
	size_t			n;

	n = 0;
	res = "";
	while ((n = read(fd, buff, sizeof(buff))) > 0)
		res.append(buff, n);
	return (res);
}
