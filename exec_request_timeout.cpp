/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_request_timeout.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 10:39:04 by msalohy           #+#    #+#             */
/*   Updated: 2025/08/13 12:57:09 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
void    Client::read_timeout()
{
    std::string head;
	int fd;
    std::stringstream ss;
	std::string exec;

	fd =0;
	head = "";
	if (access(config.get_errors().get_path_408().c_str(),F_OK | R_OK) < 0)
	{
		head = "<center><h1>408  Request timeout</h1></center>";
	}
	else
	{
		fd = fd_is_ready(config.get_errors().get_path_408(),polls,fd_wait);
    	// std::cout << config.get_errors().get_path_400() << "===" <<fd<< std::endl;
		if (fd == -1)
        	throw NotReady("408");
    	head = get_html_page(fd);
		fd_closed(fd,polls,fd_wait,config.get_errors().get_path_408());
	}
	ss << head.size();
	exec = "HTTP/1.1 408 KO\r\nContent-Length: "+ss.str()+"\r\nContent-Type: text/html\r\n\r\n"+ head;
	if ((this->polls->get_status(socket) & POLLOUT) && ! (this->polls->get_status(socket) & POLLHUP))
        send(socket, exec.c_str(), exec.size(), 0);
}
void    Client::exec_request_timeout()
{
    try
    {
        read_timeout();
        if (fd_wait.size() == 0)
		{
            stat = -1;
			requette = "";
			body = "";
			std::cout << "vita" << std::endl;
		}
    }
    catch(NotReady &e)
    {
        (void)e;
    }
}