/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExecUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 12:29:23 by jerrandr          #+#    #+#             */
/*   Updated: 2025/08/05 17:56:57 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecUtils.hpp"
#include <sys/stat.h>
#include <fcntl.h>

ExecUtils::ExecUtils()
{
	Er.insert(std::pair<std::string, std::string>("400", "bad request"));
	Er.insert(std::pair<std::string, std::string>("403", "forbidden"));
	Er.insert(std::pair<std::string, std::string>("404", "not found"));
	Er.insert(std::pair<std::string, std::string>("405", "method not allowed"));
	Er.insert(std::pair<std::string, std::string>("406", "not acceptable"));
	Er.insert(std::pair<std::string, std::string>("408", "request timeout"));
	Er.insert(std::pair<std::string, std::string>("410", "gone"));
	Er.insert(std::pair<std::string, std::string>("411", "length required"));
	Er.insert(std::pair<std::string, std::string>("413", "playload too large"));
	Er.insert(std::pair<std::string, std::string>("414", "uri too long"));
	Er.insert(std::pair<std::string, std::string>("415", " unsupported media type"));
	Er.insert(std::pair<std::string, std::string>("417", "expectation failed"));
	Er.insert(std::pair<std::string, std::string>("500", "internal server error"));
	Er.insert(std::pair<std::string, std::string>("501", "method not implemented"));
	Er.insert(std::pair<std::string, std::string>("502", "bad gateway"));
	Er.insert(std::pair<std::string, std::string>("503", "service unavailable"));
	Er.insert(std::pair<std::string, std::string>("504", "Gateway timeout"));
	Er.insert(std::pair<std::string, std::string>("505", "http version not supported"));
}

ExecUtils::~ExecUtils() {}

std::string	ExecUtils::ToString(int nbr)
{
	std::stringstream	convert;
	std::string			res;

	convert << nbr;
	convert >> res;
	return (res);
}

std::string		ExecUtils::getStatus(std::string fl)
{
	std::string	res;

	res = fl.substr(6, fl.find("."));
	return (res);
}

std::string		ExecUtils::getErrorUtils(std::string st)
{
	std::string res;
	std::string	status;
	std::string	ct;
	std::string	nbr;
	
	ct = getData(st);
	nbr = ToString(ct.size());
	status = getStatus(st);
	res = "HTTP/1.1 " + status + Er[status] + "\r\nContent-Length: " + nbr + "\r\nContent-Type: text/html\r\n\r\n" + ct;
	return (res);
}

std::string	ExecUtils::getError(std::string filename)
{
	std::string					rp;
	std::vector<std::string>	dt;
	
	rp = "";
	for (size_t i = 3; i < 7; i++)
		dt.push_back("error/40" + ToString(i) + ".html");
	for (size_t i = 0; i < 6; i++)
		dt.push_back("error/50" + ToString(i) + ".html");
	dt.push_back("error/400.html");
	dt.push_back("error/408.html");
	dt.push_back("error/411.html");
	dt.push_back("error/413.html");
	dt.push_back("error/414.html");
	dt.push_back("error/415.html");
	dt.push_back("error/417.html");

	if (std::find(dt.begin(), dt.end(), filename) != dt.end())
		rp = getErrorUtils(filename);
	return (rp);
}

std::string ExecUtils::getData(std::string filename)
{
	std::string			data;
	struct stat			st;
	int					fd;

	std::cout << "FILENAME: " << filename << std::endl;
	data = "";
	if (stat(filename.c_str(), &st) == -1)
		data = getData("error/404.html");
	else if (access(filename.c_str(), O_RDWR) != 0)
		data = getData("error/403.html");
	else
	{
		fd = open(filename.c_str(), O_RDONLY);
		data = getData(fd);
		close(fd);
	}	
	return (data);
}

std::string	ExecUtils::getData(int fd)
{
	std::string	res;
	char		buff[1024];
	int			n;

	n = 4;
	res = "";
	while ((n = read(fd, buff, sizeof(buff))) > 0)
		res.append(buff, n);
	return (res);
}
