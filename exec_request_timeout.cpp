/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_request_timeout.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 10:39:04 by msalohy           #+#    #+#             */
/*   Updated: 2025/09/25 08:57:35 by jerrandr         ###   ########.fr       */
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
		if (fd == -1)
        	throw NotReady("408");
    	head = get_html_page(fd);
		fd_closed(fd,polls,fd_wait,config.get_errors().get_path_408());
	}
	ss << head.size();
	exec = "HTTP/1.1 408 KO\r\nContent-Length: "+ss.str()+"\r\nContent-Type: text/html\r\n\r\n"+ head;
	if ((this->polls->get_status(socket) & POLLOUT) && ! (this->polls->get_status(socket) & POLLHUP))
    {
		if (send(socket, exec.c_str(), exec.size(), 0) < 0)
			stat = -1;
	}
}
void    Client::exec_request_timeout()
{
    try
    {
        read_timeout();
        if (fd_wait.size() == 0 && polls->get_new_fd_poll() <= 0 && !fl)
		{
            stat = -1;
			request = "";
			body = "";
		}
    }
    catch(NotReady &e)
    {
        (void)e;
    }
}