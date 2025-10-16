/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   len_required.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 11:35:23 by msalohy           #+#    #+#             */
/*   Updated: 2025/10/02 18:44:04 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

bool    Client::is_post()
{
    std::string nw;
    std::vector <std::string> tab;

    nw = request.substr(0,request.find("\r\n"));
    tab = split(nw," ");
    if (tab.size()>0 && tab[0] == "POST")
        return true;
    return false;
}

bool    Client::is_len_required()
{
    if (is_post() && real_body == -1 && !is_chunked(request))
        return true;
    return false;
}

void    Client::exec_len_required()
{
    std::string head;
	int fd;
    std::stringstream ss;
	std::string exec;

	fd =0;
	head = "";
	if (access(config.get_errors().get_path_411().c_str(),F_OK | R_OK) < 0)
	{
		head = "<center><h1>411 Length Required</h1></center>";
	}
	else
	{
		fd = fd_is_ready(config.get_errors().get_path_411(),polls,fd_wait);
		if (fd == -1)
        	throw NotReady("411");
    	head = get_html_page(fd);
		fd_closed(fd,polls,fd_wait,config.get_errors().get_path_411());
	}
	ss << head.size();
	exec = "HTTP/1.1 411 KO\r\nContent-Length: "+ss.str()+"\r\nContent-Type: text/html\r\n\r\n"+ head;
	if ((this->polls->get_status(socket) & POLLOUT) && ! (this->polls->get_status(socket) & POLLHUP))
    {
		if (send(socket, exec.c_str(), exec.size(), MSG_NOSIGNAL) < 0)
			stat = -1;
	}
}