/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyUpload.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 13:49:28 by jerrandr          #+#    #+#             */
/*   Updated: 2025/08/13 14:28:48 by jerrandr         ###   ########.fr       */
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
	int			start;
	int			end;

	start = 0;
	end = 0;
	res = "";
	std::cout << "HEADER: {" << header << "}" << std::endl;
	if (header.find("filename") != std::string::npos)
	{
		res = header.substr(header.find("filename"), header.find("\r\n"));
		start = res.find("\"");
		res = res.substr(start + 1, res.length());
		end = res.find("\"");
		res = res.substr(0, end);
		// start = 0;
		// if (res.find(".") != std::string::npos)
		// {
		// 	for (size_t i = res.length(); i > 0; i--)
		// 	{
		// 		if (res[i] == '.' && start == 0)
		// 			start = i;
		// 	}
		// 	res = res.substr(start, res.length() - 1);
		// }
		// else
		// 	res = "";
	}
	return (res);
}

void BodyUpload::ParseBody()
{
	std::string		header;
	std::string		ct;
	std::string		filename;
	std::ofstream	file;

	ct = "";
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
		if (header.find("filename") != std::string::npos)
			header = ParseHeader(header);
		else
			continue ;
		if (header == "")
			break ; 
		filename = (header + "(Upload)");
		file.open(filename.c_str());
		if (file.fail())
			break ;
		file << ct;
		file.close();
	}
}
