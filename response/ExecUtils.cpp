/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExecUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 12:29:23 by jerrandr          #+#    #+#             */
/*   Updated: 2025/09/17 09:00:35 by jerrandr         ###   ########.fr       */
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
	Er.insert(std::pair<std::string, std::string>("415", "unsupported media type"));
	Er.insert(std::pair<std::string, std::string>("417", "expectation failed"));
	Er.insert(std::pair<std::string, std::string>("500", "internal server error"));
	Er.insert(std::pair<std::string, std::string>("501", "method not implemented"));
	Er.insert(std::pair<std::string, std::string>("502", "bad gateway"));
	Er.insert(std::pair<std::string, std::string>("503", "service unavailable"));
	Er.insert(std::pair<std::string, std::string>("504", "Gateway timeout"));
	Er.insert(std::pair<std::string, std::string>("505", "http version not supported"));
	Erro = "<!DOCTYPE html><html lang=\"en\"><head>    <meta charset=\"UTF-8\">    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">    <title>Document</title></head><body>    <center><h1>500 Internal Server Error</h1></center></body></html>";
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

std::string	ExecUtils::getError(std::string filename, Client &cl)
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
		rp = getErrorUtils(filename, cl);
	return (rp);
}

std::string	ExecUtils::getExt(std::string filename)
{
	std::string res;
	int			st;
	int			fl;

	st = 0;
	fl = 0;
	res = "";
	if (filename.find(".") != std::string::npos)
	{
		for (size_t i = filename.length(); i > 0; i--)
		{
			if (filename[i] == '.' && fl == 0)
			{
				st = i;
				fl = 1;
				break;
			}
		}
		res = filename.substr(st, filename.length());
	}
	return (res);
}

bool	ExecUtils::checkTimeOut(time_t begin, time_t end)
{
	time_t tmp;

	tmp = end - begin;
	std::cout << "TMP {" << tmp << "}\n";
	if (tmp >= 58)
		return (true);
	return (false);
}

std::string	ExecUtils::CheckError(std::string	rp, Client &Cl)
{
	std::string	res;

	res = "";
	if (access(rp.c_str(), F_OK) == -1)
		res = getError("error/404.html", Cl);
	else if (access(rp.c_str(), R_OK) == -1)
		res = getError("error/403.html", Cl);
	return (res);	
}

void	ExecUtils::SendResponse(Pollfd *pl, std::string rp, int fdc)
{
	size_t	pc;
	size_t	len;

	pc = 2048;
	if ((pl->get_status(fdc) & POLLOUT) && !(pl->get_status(fdc) & POLLHUP))
	{
		for (size_t i = 0; i < rp.size(); i += pc)
		{
				len = std::min(pc, rp.size() - i);
				send(fdc, rp.c_str(), len, 0);
		}
	}
}