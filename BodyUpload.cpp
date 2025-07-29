/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyUpload.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 13:49:28 by jerrandr          #+#    #+#             */
/*   Updated: 2025/07/29 09:15:07 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BodyUpload.hpp"
#define RED "\033[31m"
#define R "\033[0m"

BodyUpload::~BodyUpload() {}

BodyUpload::BodyUpload(std::string body) : Bdy(body) {}

void BodyUpload::ParseBody()
{
	std::string	header;
	std::string	sep;
	int			fl;

	fl = 1;
	sep = "";

	while (Bdy.find("\r\n\r\n") != std::string::npos)
	{
		sep = Bdy.substr(Bdy.find("\r\n\r\n") + 4, Bdy.length());
		sep = sep.substr(0, sep.find("------WebKit"));
		Bdy = Bdy.substr(Bdy.find(sep) + sep.length(), Bdy.length());
		if (sep == "\r\n\r\n\r\n")
			break;
		std::ofstream	file("file" + utils.ToString(fl) + ".txt");
		if (file.fail())
			break ;
		file << sep;
		file.close();
		fl++;
	}
}
