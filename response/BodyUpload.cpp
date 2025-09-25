/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyUpload.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 13:49:28 by jerrandr          #+#    #+#             */
/*   Updated: 2025/09/25 09:02:52 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BodyUpload.hpp"
#define RED "\033[31m"
#define R "\033[0m"

BodyUpload::~BodyUpload() {}

BodyUpload::BodyUpload(std::string rt) : Rt(rt) {}

std::string	BodyUpload::ParseHeader(std::string header)
{
	std::string res;
	int			start;
	int			end;

	start = 0;
	end = 0;
	res = "";
	if (header.find("filename") != std::string::npos)
	{
		res = header.substr(header.find("filename"), header.find("\r\n"));
		start = res.find("\"");
		res = res.substr(start + 1, res.length());
		end = res.find("\"");
		res = res.substr(0, end);
	}
	return (res);
}

std::string	BodyUpload::getSep(std::string bd)
{
	std::string	res;

	res = bd.substr(0, bd.find("\r\n"));
	return (res);
}

int  BodyUpload::fd_create(std::string path, Pollfd *polls, std::map<std::string, int> &fd_wait)
{
    int fd;

    fd = -1;
    try
    {
        fd = fd_wait.at(path);
        if ((polls->get_status(fd) & POLLIN))
            return fd;
    }
    catch(const std::out_of_range &e)
    {
		fd = open(path.c_str(), O_CREAT | O_WRONLY, 0644);
        if (fd < 0)
            throw std::bad_alloc();
        (void)e;
        polls->add_new_fd(fd);
        fd_wait[path] = fd;
    }
    return -1;
}

void BodyUpload::ParseBody(Client &cl)
{
	std::string		header;
	std::string		ct;
	std::string		filename;
	std::string		sep;
	std::string		Bdy;
	
	Bdy = cl.getBody();
	sep = getSep(Bdy);
	
	while (true)
	{
		if (Bdy.find("\r\n\r\n") == std::string::npos)
			break;
		header = Bdy.substr(0, Bdy.find("\r\n\r\n"));
		Bdy = Bdy.substr(header.length() + 4, Bdy.length());
		ct = Bdy.substr(0, (Bdy.find(sep) - 1));
		header = ParseHeader(header);
		filename = Rt + header;
		Bdy = Bdy.substr(ct.size() + 4, Bdy.length());
		if (header != "")
		{
			fd_create(filename, cl.getPoll(), cl.getFdWait());
			vl.insert(std::pair<std::string, std::string>(filename, ct));
		}
	}	
}

void	BodyUpload::UploadHandler(Client &cl)
{
	int	fd;

	fd = 0;
	ParseBody(cl);
	for (std::map<std::string, std::string>::iterator i = vl.begin(); i != vl.end(); i++)
	{
		if ((fd = fd_create((*i).first, cl.getPoll(), cl.getFdWait())) != -1)
		{
			write(fd, (*i).second.data(), (*i).second.size());
			fd_closed(fd, cl.getPoll(), cl.getFdWait(), (*i).first);
		}
	}	
}