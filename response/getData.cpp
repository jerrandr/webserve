/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getData.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 18:04:38 by jerrandr          #+#    #+#             */
/*   Updated: 2025/09/22 11:49:03 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ExecUtils.hpp"

std::string ExecUtils::getData(std::string filename, Client & cl)
{
	std::string			data;
	int					fd;

	data = "";
	fd = fd_is_ready(filename, cl.getPoll(), cl.getFdWait());
	if (fd == -1)
		throw NotReady("TSY METYYYYYYYYYYYYYYYYYY");
	data = getData(fd);
	fd_closed(fd, cl.getPoll(), cl.getFdWait(),filename);
	return (data);
}

std::string	ExecUtils::getData(int fd)
{
	std::string	res;
	char		buff[1024];
	size_t			n;

	n = 4;
	res = "";
	while ((n = read(fd, buff, sizeof(buff))) > 0)
	{
		res.append(buff, n);
		std::cerr << "n : " << n << "\n";
	}
	return (res);
}
