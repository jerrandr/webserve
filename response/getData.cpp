/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getData.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 18:04:38 by jerrandr          #+#    #+#             */
/*   Updated: 2025/09/13 14:14:50 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecUtils.hpp"

std::string ExecUtils::getData(std::string filename, Client & cl, int &fl)
{
	std::string			data;
	struct stat			st;
	int					fd;

	std::cout << "FILENAME: " << filename << std::endl;
	data = "";
	if (stat(filename.c_str(), &st) == -1)
	{
		fl = -1;
		data = getData("error/404.html", cl, fl);
	}
	else if (access(filename.c_str(), O_RDWR) != 0)
	{
		fl = -1;
		data = getData("error/403.html", cl, fl);
	}
	else
	{
		fd = fd_is_ready(filename, cl.getPoll(), cl.getFdWait());
		if (fd == -1)
			throw NotReady("TSY METYYYYYYYYYYYYYYYYYY");
		data = getData(fd);
		fd_closed(fd, cl.getPoll(), cl.getFdWait(),filename);
	}
	return (data);
}

std::string	ExecUtils::getData(int fd)
{
	std::string	res;
	char		buff[1024];
	size_t			n;

	n = 4;
	res = "";
	while ((n = read(fd, buff, sizeof(buff))) > 0)
			res.append(buff, n);
	return (res);
}

std::string		ExecUtils::getErrorUtils(std::string st,  Client & cl)
{
	std::string res;
	std::string	status;
	std::string	ct;
	std::string	nbr;
	int			fl;

	fl = 0;
	ct = getData(st, cl, fl);
	if (fl == -1)
		ct = Erro;
	nbr = ToString(ct.size());
	status = getStatus(st);
	res = "HTTP/1.1 " + status + Er[status] + "\r\nContent-Length: " + nbr + "\r\nContent-Type: text/html\r\n\r\n" + ct;
	return (res);
}