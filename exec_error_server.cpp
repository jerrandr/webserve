/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error_server.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 13:19:49 by msalohy           #+#    #+#             */
/*   Updated: 2025/10/04 14:19:55 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
void    Client::error_serv()
{
    std::string head;
	int fd;
    std::stringstream ss;
	std::string exec;

	fd =0;
	head = "";
	if (access(config.get_errors().get_path_500().c_str(),F_OK | R_OK) < 0)
	{
		head = "<center><h1>500 internal server error</h1></center>";
	}
	else
	{
		fd = fd_is_ready(config.get_errors().get_path_500(),polls,fd_wait);
		if (fd == -1)
        	throw NotReady("500");
    	head = get_html_page(fd);
		close(fd);
		fd_closed(fd,polls,fd_wait,config.get_errors().get_path_500());
	}
	ss << head.size();
	exec = "HTTP/1.1 500 KO\r\nContent-Length: "+ss.str()+"\r\nContent-Type: text/html\r\n\r\n"+ head;
	if ((this->polls->get_status(socket) & POLLOUT) && ! (this->polls->get_status(socket) & POLLHUP))
    {
		if (send(socket, exec.c_str(), exec.size(), MSG_NOSIGNAL) <= 0)
			stat = -1;
	}
}

void    Client::exec_500()
{
	std::string head;
    std::stringstream ss;
	std::string exec;

	head = "";
	head = "<center><h1>500 internal server error</h1></center>";
	ss << head.size();
	exec = "HTTP/1.1 500 KO\r\nContent-Length: "+ss.str()+"\r\nContent-Type: text/html\r\n\r\n"+ head;
	if ((this->polls->get_status(socket) & POLLOUT) && ! (this->polls->get_status(socket) & POLLHUP))
    {
		if (send(socket, exec.c_str(), exec.size(), MSG_NOSIGNAL) <= 0)
			stat = -1;
	
	}
}
void    Client::exec_error_server()
{
     try
    {
        error_serv();
        if ((fd_wait.size() == 0 && get_len_fd() <= 0 && !fl))
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