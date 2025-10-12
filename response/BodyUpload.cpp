/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyUpload.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 13:49:28 by jerrandr          #+#    #+#             */
/*   Updated: 2025/10/11 19:37:55 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecUtils.hpp"
#include "BodyUpload.hpp"
#define RED "\033[31m"
#define R "\033[0m"

BodyUpload::~BodyUpload()
{
	delete utils;
}

BodyUpload::BodyUpload(std::string rt) : Rt(rt)
{
	utils = new ExecUtils();
}

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
		fd = open(path.c_str(), O_CREAT | O_WRONLY, 0666);
        if (fd < 0)
			throw std::bad_alloc();
        (void)e;
        polls->add_new_fd(fd);
        fd_wait[path] = fd;
    }
    return -1;
}

std::string	BodyUpload::answer(std::string rt, Client &Cl)
{
	// std::stringstream	data;
	// std::string	ext;
	std::string	nbr;
	std::string	rp;
	// std::string	body;

	(void)rt;
	(void)Cl;
	// body = "<!DOCTYPE html><html lang=\"en\"><head>    <meta charset=\"UTF-8\">    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">    <title>Document</title></head><body>    <h1>UPLOAD SUCCES</h1></body></html>";
	// ext = ".html";
	// std::cout << "RT {" << rt << "}\n";
	// data << utils->CheckError(rt, Cl);
	// if (data.str() == "")
	// {
	// 	data.clear();
	// 	data << utils->getData(rt, Cl);
	// 	ext = utils->getExt(rt);
	// }
	// else
	// {
	// 	rp = data.str();
	// 	return (rp);
	// }
	// nbr = utils->ToString(body.size());
	rp = "HTTP/1.1 201 Created\r\nContent-Length: 0\r\n\r\n";
	return (rp);
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
	
	while (Bdy.find("\r\n\r\n") != std::string::npos)
	{
		header = Bdy.substr(0, Bdy.find("\r\n\r\n"));
		Bdy = Bdy.substr(header.length() + 4, Bdy.length());
		ct = Bdy.substr(0, (Bdy.find(sep) - 1));
		header = ParseHeader(header);
		std::cout << "HEADER {" << header << "}\n";
		filename = Rt + header;
		if (Bdy.length() < ct.size() + 4)
			break;
		
		Bdy = Bdy.substr(ct.size() + 4, Bdy.length());
		if (header != "")
		{
			filPath.push_back(header);
			fd_create(filename, cl.getPoll(), cl.getFdWait());
			std::cout << "CT {" << ct << "}\n";
			std::cout << "FILENAME {" << filename << "}\n";
			vl.insert(std::pair<std::string, std::string>(filename, ct));
		}
	}	
}

void	BodyUpload::UploadHandler(Client &cl)
{
	int	fd;

	fd = 0;
	ParseBody(cl);
	if (vl.size() > 0)
	{
		for (std::map<std::string, std::string>::iterator i = vl.begin(); i != vl.end(); i++)
		{
			
			if ((fd = fd_create((*i).first, cl.getPoll(), cl.getFdWait())) != -1)
			{
				write(fd, (*i).second.data(), (*i).second.size());
				fd_closed(fd, cl.getPoll(), cl.getFdWait(), (*i).first);
			}
		}	
	}
}