/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyUpload.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 13:49:28 by jerrandr          #+#    #+#             */
/*   Updated: 2025/09/08 10:37:33 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BodyUpload.hpp"
#define RED "\033[31m"
#define R "\033[0m"

BodyUpload::~BodyUpload() {}

BodyUpload::BodyUpload(std::string body, std::string rt) : Bdy(body), Rt(rt) {}

std::string	BodyUpload::ParseHeader(std::string header)
{
	std::string res;
	int			start;
	int			end;

	start = 0;
	end = 0;
	res = "";
	// std::cout << "HEADER: {" << header << "}" << std::endl;
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

void BodyUpload::ParseBody(Client &cl)
{
	std::string		header;
	std::string		ct;
	std::string		filename;
	int				fd;
	// std::ofstream	file;
	ct = "";
	fd = 0;
	while (Bdy.find("\r\n\r\n") != std::string::npos)
	{
		header = Bdy.substr(0, Bdy.find("\r\n\r\n"));
		ct = Bdy.substr(Bdy.find("\r\n\r\n") + 4, Bdy.length());
		if (ct.find("------WebKit") == 0)
		{
			Bdy = ct;
			ct = "";
		}
		else
		{
			ct = ct.substr(0, ct.find("------WebKit"));
			Bdy = Bdy.substr(Bdy.find(ct) + ct.length(), Bdy.length());
		}
		if (header == "")
			break ;
		header = ParseHeader(header);
		filename = (Rt + header);
		std::cout << RED << "FILENAME: {" << filename << "}\n" << R;
		fd = fd_is_ready(filename, cl.getPoll(), cl.getFdWait());
		if (fd == -1)
			break ;
		write(fd, ct.c_str(), ct.size());
		// file.open(filename.c_str());
		// if (file.fail())
		// 	break ;
		// file << ct;
		// file.close();
	}
}
