/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyUpload.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 13:49:28 by jerrandr          #+#    #+#             */
/*   Updated: 2025/08/04 09:59:44 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BodyUpload.hpp"
#define RED "\033[31m"
#define R "\033[0m"

BodyUpload::~BodyUpload() {}

BodyUpload::BodyUpload(std::string body) : Bdy(body) {}

std::string	BodyUpload::ParseHeader(std::string header)
{
	std::string res;

	res = "";
	if (header.find("filename") != std::string::npos)
		res = header.substr(header.find("filename"), header.find("\r\n"));
	res = res.substr(res.find("."), res.length());
	res = res.substr(0, res.find("\""));
	return (res);
}

void BodyUpload::ParseBody()
{
	std::string		header;
	std::string		ct;
	std::string		filename;
	std::ofstream	file;
	int				fl;

	fl = 1;
	ct = "";
	while (Bdy.find("\r\n\r\n") != std::string::npos)
	{
		header = Bdy.substr(0, Bdy.find("\r\n\r\n"));
		header = ParseHeader(header);
		std::cout << "header: " << header << std::endl;
		ct = Bdy.substr(Bdy.find("\r\n\r\n") + 4, Bdy.length());
		ct = ct.substr(0, ct.find("------WebKit"));
		Bdy = Bdy.substr(Bdy.find(ct) + ct.length(), Bdy.length());
		if (ct == "\r\n\r\n\r\n")
			break;
		filename = ("file" + utils.ToString(fl) + header); 
		file.open(filename.c_str());
		if (file.fail())
			break ;
		file << ct;
		file.close();
		fl++;
	}
}
