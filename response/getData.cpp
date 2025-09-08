/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getData.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 18:04:38 by jerrandr          #+#    #+#             */
/*   Updated: 2025/09/08 08:55:07 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecUtils.hpp"

std::string ExecUtils::getData(std::string filename, Pollfd *polls, std::map<std::string, int> &fd_wait, int &fl)
{
	std::string			data;
	struct stat			st;
	int					fd;

	std::cout << "FILENAME: " << filename << std::endl;
	data = "";
	if (stat(filename.c_str(), &st) == -1)
	{
		fl = -1;
		data = getData("error/404.html", polls, fd_wait, fl);
	}
	else if (access(filename.c_str(), O_RDWR) != 0)
	{
		fl = -1;
		data = getData("error/403.html", polls, fd_wait, fl);
	}
	else
	{
		fd = fd_is_ready(filename, polls, fd_wait);
		if (fd == -1)
			throw NotReady("TSY METYYYYYYYYYYYYYYYYYY");
		data = getData(fd);
		fd_closed(fd,polls,fd_wait,filename);
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

std::string		ExecUtils::getErrorUtils(std::string st,  Pollfd *polls, std::map<std::string, int> &fd_wait)
{
	std::string res;
	std::string	status;
	std::string	ct;
	std::string	nbr;
	int			fl;

	fl = 0;
	ct = getData(st, polls, fd_wait, fl);
	if (fl == -1)
		ct = Erro;
	nbr = ToString(ct.size());
	status = getStatus(st);
	res = "HTTP/1.1 " + status + Er[status] + "\r\nContent-Length: " + nbr + "\r\nContent-Type: text/html\r\n\r\n" + ct;
	return (res);
}