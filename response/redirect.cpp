/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:56:34 by jerrandr          #+#    #+#             */
/*   Updated: 2025/10/07 13:46:02 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::string	Response::redir_rp2(std::string redir)
{
	std::vector<std::string> data;
	std::stringstream		convert;
	std::string				rp = "";
	int status = 0;

	data = split(redir, " ");
	convert << data[0];
	convert >> status;
	
	if (data.size() == 2)
	{
		switch (status)
		{
			case 301:
				rp = "HTTP/1.1 301 Moved Permanently\r\n";
				break;
			case 302:
				rp = "HTTP/1.1 302 Found\r\n";
				break;
			case 307:
				rp = "HTTP/1.1 307 Temporary Redirect\r\n";
				break;
			case 308:
				rp = "HTTP/1.1 308 Permanent Redirect\r\n";
				break;
			default:
				rp = "HTTP/1.1 301 Moved Permanently\r\n";
				break;
		}
		rp += "Location: " + data[1] + "\r\nContent-Length: 0\r\n\r\n"; 
	}
	return (rp);
}


void	Response::redir_rp(std::string redir, int socket)
{
	std::string	rp;

	rp = redir_rp2(redir);
	if (rp != "")
	{
		utils->SendResponse(Cl, rp, socket);
		return ;
	}
}