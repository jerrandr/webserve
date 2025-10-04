/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunked.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 08:43:23 by msalohy           #+#    #+#             */
/*   Updated: 2025/09/19 13:33:11 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"



void Client::body_unchunked()
{
	long current_size;
	std::string tmp;
	std::string new_body;
	std::size_t temp;

	current_size = 0;
	tmp = "";
	temp = 0;
	new_body = "";
	real_body = -1;
	for (std::size_t i = 0; i < body.size(); i++)
	{
		temp = body.find("\r\n", i);
		if (temp == std::string::npos)
		{
			break;
		}
		tmp = body.substr(i, (temp - i));
		current_size = strtol(tmp.c_str(), NULL, 16);
		real_body += current_size;
		i = temp;
		i += 2;
		for (long j = 0; j < (current_size); j++)
			new_body += body[j + i];
		i += current_size + 1;
		current_size = 0;
		tmp = "";
		temp = 0;
		if (i >= body.size())
			break;
	}
	body = new_body;
}