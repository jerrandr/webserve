/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExecUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 12:29:23 by jerrandr          #+#    #+#             */
/*   Updated: 2025/10/18 08:31:19 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecUtils.hpp"
#include <sys/stat.h>
#include <fcntl.h>
#include <algorithm>

ExecUtils::~ExecUtils() 
{
	Er.clear();
}

ExecUtils::ExecUtils()
{
	Er.insert(std::pair<std::string, std::string>("400", "bad request"));
	Er.insert(std::pair<std::string, std::string>("403", "forbidden"));
	Er.insert(std::pair<std::string, std::string>("404", "not found"));
	Er.insert(std::pair<std::string, std::string>("405", "method not allowed"));
	Er.insert(std::pair<std::string, std::string>("406", "not acceptable"));
	Er.insert(std::pair<std::string, std::string>("408", "request timeout"));
	Er.insert(std::pair<std::string, std::string>("409", "Conflict"));
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

std::string	ExecUtils::getFilenameError(int st, const ErrorPage & ep)
{
	std::string filename;

	switch (st)
	{
		case 400:
			filename = ep.get_path_400();
			break;
		case 403:
			filename = ep.get_path_403();
			break;
		case 404:
			filename = ep.get_path_404();
			break;
		case 405:
			filename = ep.get_path_405();
			break;
		case 406:
			filename = ep.get_path_406();
			break;
		case 408:
			filename = ep.get_path_408();
			break;
		case 409:
			filename = ep.get_path_409();
			break;
		case 410:
			filename = ep.get_path_410();
			break;
		case 411:
			filename = ep.get_path_411();
			break;
		case 413:
			filename = ep.get_path_413();
			break;		
		case 414:
			filename = ep.get_path_414();
			break;
		case 415:
			filename = ep.get_path_415();
			break;		
		case 417:
			filename = ep.get_path_417();
			break;
		case 500:
			filename = ep.get_path_500();
			break;
		case 501:
			filename = ep.get_path_501();
			break;
		case 502:
			filename = ep.get_path_502();
			break;
		case 503:
			filename = ep.get_path_503();
			break;
		case 504:
			filename = ep.get_path_504();
			break;
		case 505:
			filename = ep.get_path_505();
			break;
		default:
			filename = ep.get_path_500();
	}
	return (filename);
}

std::string	ExecUtils::getError(Client &cl, int stat)
{
	std::string 								res;
	std::string									status;
	std::string									ct;
	std::string									nbr;
	std::string									filename;
	std::vector<std::pair<int, std::string> >	checker;
	std::string									ext;
	Config										cf;

	cf = cl.getConfig();
	filename = getFilenameError(stat, cf.get_errors());
	checker = get_403_404(filename);
	if (checker[0].first !=  0)
	{
		ct = checker[0].second;
		status = checker[0].first;
		ext = ".html";
	}
	else
	{
		ct = getData(filename, cl);
		status = ToString(stat);
		ext = getExt(filename);
	}
	nbr = ToString(ct.size());
	res = "HTTP/1.1 " + status +" "+ Er[status] + "\r\nContent-Length: " + nbr + "\r\nContent-Type: " + cf.get_mime(ext) + "\r\n\r\n" + ct;
	return (res);
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
	if (tmp >= 60)
		return (true);
	return (false);
}

std::string	ExecUtils::CheckError(std::string	rp, Client &Cl)
{
	std::string	res;

	res = "";
	if (access(rp.c_str(), F_OK) == -1)
		res = getError(Cl, 404);
	else if (access(rp.c_str(), R_OK) == -1)
		res = getError(Cl, 403);
	return (res);	
}

void	ExecUtils::SendResponse(Client & Cl, std::string &rp, int fdc)
{
	Pollfd		*pl = Cl.getPoll();
	size_t		pt = 1048576;
	size_t		sentNow = 0;

	if (Cl.sd == NULL)
	{
		Cl.sd = new Send(rp);
		pl->set_new_fd();
		Cl.len_inc();
	}
	std::string	& data = Cl.sd->getData();
	pt = std::min(pt, data.length());
	if (pt > 0)
	{		
		if ((pl->get_status(fdc) & POLLOUT) && !(pl->get_status(fdc) & POLLHUP))
		{
			if ((sentNow = send(fdc, data.c_str(),pt, MSG_NOSIGNAL)) <= 0)
			{
				Cl.set_status_client(-1);
				pl->decrement_new_fd();
				Cl.len_dec();
				delete Cl.sd;
				Cl.sd = NULL;
				pt = 0;
				return ;
			}
			Cl.sd->sent += sentNow;
			data = data.substr(sentNow);
			if (Cl.sd->getSize() > Cl.sd->sent)
			{
				throw NotReady();
			}
			else
			{
			pl->decrement_new_fd();
			Cl.len_dec();
			delete Cl.sd;
			Cl.sd = NULL;
			}
		}
	}
	else
	{
		pl->decrement_new_fd();
		Cl.len_dec();
		delete Cl.sd;
		Cl.sd = NULL;
	}
}

std::string ExecUtils::getRealPath(std::string uri, Location loc)
{
    std::string real;

	real = loc.get_root();
	real += uri;
    return (real);
}

std::string	ExecUtils::getQuery(std::string str)
{
	std::string					qr;
	
	qr = "";
	if (str.find("?") == std::string::npos)
		return (qr);
	qr = str.substr(str.find("?") + 1, str.length());
	return (qr);	
}