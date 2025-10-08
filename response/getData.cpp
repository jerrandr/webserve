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
	getData1(fd, cl);
	if (!cl.st)
		throw NotReady();
	cl.st = false;
	data.assign(cl.data.data(), cl.data.size());
	cl.data.clear();
	fd_closed(fd, cl.getPoll(), cl.getFdWait(),filename);
	return (data);
}


void	ExecUtils::getData1(int fd, Client & cl)
{
	std::vector<char> &data = cl.data;
	char		buff[1048576];
	size_t			n;

	n = 0;
	if ((n = read(fd, buff, sizeof(buff))) > 0)
		data.insert(data.end(), buff, buff + n);
	else
		cl.st = true;
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
