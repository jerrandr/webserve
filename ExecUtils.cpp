/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExecUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 12:29:23 by jerrandr          #+#    #+#             */
/*   Updated: 2025/07/22 08:40:42 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecUtils.hpp"
ExecUtils::ExecUtils() {}

ExecUtils::~ExecUtils() {}

std::string	ExecUtils::ToString(int nbr)
{
	std::stringstream	convert;
	std::string			res;

	convert << nbr;
	convert >> res;
	return (res);
}

std::stringstream ExecUtils::getData(std::string filename)
{
	std::ifstream		file(filename);
	std::stringstream	data;

	std::cout << "FILENAME: " << filename << std::endl;
	data << "";
	if (file.fail())
		std::cout << "ERROR" << std::endl;
	data << file.rdbuf();
	return (data);
}

std::string	ExecUtils::getData(int fd)
{
	std::string	res;
	char		buff[1024];
	int			n;

	n = 0;
	memset(buff, 0, 1024);
	res = "";
	while ((n = read(fd, buff, sizeof(buff))) > 0)
		res.append(buff, n);
	return (res);
}
