/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bad_request.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 08:42:08 by msalohy           #+#    #+#             */
/*   Updated: 2025/08/13 12:55:16 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

bool    Client::is_bad_request()
{
	std::string tmp;
	std::size_t start;
	std::size_t end;
	std::vector<std::string> f;


	tmp = "";
	start = 0;
	end = 0;
	
	while(start < requette.size())
	{
		end = requette.find("\r\n",start);
		if (end == std::string::npos)
			return true;
		tmp = requette.substr(start, end-start);
		if (start == 0)
		{
			f = split(tmp," ");
			if (f.size() != 3)
				return true;
		}
		else
		{
			f = split_sep(tmp,": ");
			if (f.size() != 2 && end +2 < requette.size())
				return true;
		}
		f.clear();
		tmp = "";
		start = end + 2;
		end = 0;
	}
	return false;
}

void    Client::exec_bad_request()
{
	std::string head;
	int fd;
    std::stringstream ss;
	std::string exec;

	fd =0;
	head = "";
	if (access(config.get_errors().get_path_400().c_str(),F_OK | R_OK) < 0)
	{
		head = "<center><h1>400 Bad request</h1></center>";
	}
	else
	{
		fd = fd_is_ready(config.get_errors().get_path_400(),polls,fd_wait);
    	std::cout << config.get_errors().get_path_400() << "===" <<fd<< std::endl;
		if (fd == -1)
        	throw NotReady("400");
    	head = get_html_page(fd);
		fd_closed(fd,polls,fd_wait,config.get_errors().get_path_400());
	}
	ss << head.size();
	exec = "HTTP/1.1 400 KO\r\nContent-Length: "+ss.str()+"\r\nContent-Type: text/html\r\n\r\n"+ head;
	if ((this->polls->get_status(socket) & POLLOUT) && ! (this->polls->get_status(socket) & POLLHUP))
        send(socket, exec.c_str(), exec.size(), 0);
}