/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_processing.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 08:48:38 by msalohy           #+#    #+#             */
/*   Updated: 2025/09/19 13:36:32 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"


void Client::max_body_size_trait()
{
	int fd;
	std::string path;
	std::string t;
	std::stringstream ss;

	path = this->config.get_errors().get_path_413();
	t = "";
	fd = -1;
	if (access(path.c_str(), F_OK | R_OK) < 0)
	{
			t = "413 PLAYLOAD TOO LARGE";
			ss << t.size();
			t = "HTTP/1.1 413 KO\r\nContent-Length: " + ss.str() + "\r\nContent-Type: text/html\r\n\r\n" + t;
			if ((this->polls->get_status(socket) & POLLOUT) && !(this->polls->get_status(socket) & POLLHUP))
				send(socket, t.c_str(), t.size(), 0);
			return;
	}
	fd = fd_is_ready(path, polls, fd_wait);
	if (fd != -1)
	{
		t = get_html_page(fd);
		fd_closed(fd, polls, fd_wait, path);
		ss << t.size();
		t = "HTTP/1.1 413 KO\r\nContent-Length: " + ss.str() + "\r\nContent-Type: text/html\r\n\r\n" + t;
		if ((this->polls->get_status(socket) & POLLOUT) && !(this->polls->get_status(socket) & POLLHUP))
			send(socket, t.c_str(), t.size(), 0);
	}
}
void Client::exec_dir_listing(std::string uri)
{
	Location loc;
	std::string path;

	loc = config.get_location_match(uri);
	path = config.get_real_path(uri, loc);
	directory_listing(path,uri);
}
int Client::is_dir_listing(std::string uri)
{
	Location loc;
	std::string path;

	loc = config.get_location_match(uri);
	path = config.get_real_path(uri, loc);
	if (is_directory(path))
	{
		if (loc.get_directory_listing() && loc.get_index() == "")
			return (1);
	}
	return (0);
}
static bool   http_not_supported(std::string v)
{
	if (v != "HTTP/1.1" && v != "HTTP/1.0")
		return (true);
	return false;
}
void Client::exec_http_not_supported()
{
	std::string head;
	int fd;
    std::stringstream ss;
	std::string exec;

	fd =0;
	head = "";
	if (access(config.get_errors().get_path_505().c_str(),F_OK | R_OK) < 0)
	{
		head = "<center><h1>505 Http version not supported</h1></center>";
	}
	else
	{
		fd = fd_is_ready(config.get_errors().get_path_505(),polls,fd_wait);
		if (fd == -1)
        	throw NotReady("505");
    	head = get_html_page(fd);
		fd_closed(fd,polls,fd_wait,config.get_errors().get_path_505());
	}
	ss << head.size();
	exec = "HTTP/1.1 505 KO\r\nContent-Length: "+ss.str()+"\r\nContent-Type: text/html\r\n\r\n"+ head;
	if ((this->polls->get_status(socket) & POLLOUT) && ! (this->polls->get_status(socket) & POLLHUP))
        send(socket, exec.c_str(), exec.size(), 0);
}

bool    Client::is_not_implemented(std::map<std::string, std::string> cf)
{
	Location loc;
	std::vector<std::string> list_im;

	loc = this->config.get_location_match(cf["uri"]);
	if (loc.get_meth() == "")
		return false;
	list_im = split(loc.get_meth()," ");
	for(std::size_t i = 0; i < list_im.size();i++)
	{
		if (cf["method"] == list_im[i] && cf["method"] != "GET" && cf["method"] != "POST" && cf["method"] != "DELETE")
		{
			return true;
		}
	}
	return false;
}

void    Client::exec_not_implemented()
{
	std::string head;
	int fd;
    std::stringstream ss;
	std::string exec;

	fd =0;
	head = "";
	if (access(config.get_errors().get_path_501().c_str(),F_OK | R_OK) < 0)
	{
		head = "<center><h1>501 Method not implemented</h1></center>";
	}
	else
	{
		fd = fd_is_ready(config.get_errors().get_path_501(),polls,fd_wait);
		if (fd == -1)
        	throw NotReady("501");
    	head = get_html_page(fd);
		fd_closed(fd,polls,fd_wait,config.get_errors().get_path_501());
	}
	ss << head.size();
	exec = "HTTP/1.1 501 KO\r\nContent-Length: "+ss.str()+"\r\nContent-Type: text/html\r\n\r\n"+ head;
	if ((this->polls->get_status(socket) & POLLOUT) && ! (this->polls->get_status(socket) & POLLHUP))
        send(socket, exec.c_str(), exec.size(), 0);

}
bool    Client::other_traitment(std::map<std::string, std::string> config)
{
	if (this->config.get_max_allowed_size() < this->get_len_real_body())
	{
		max_body_size_trait();
		if (fd_wait.size() == 0)
		{
			request = "";
			body = "";
		}
		return true;
	}
	else if (is_not_implemented(config))
	{
		try
		{
			exec_not_implemented();
			if (fd_wait.size() == 0)
			{
				request = "";
				body = "";
			}
		}
		catch (NotReady &e)
		{
			(void)e;
		}
		return true;
	}
	else if (http_not_supported(config["http_version"]))
	{
		try
		{
			exec_http_not_supported();
			if (fd_wait.size() == 0)
			{
				request = "";
				body = "";
			}
		}
		catch (NotReady &e)
		{
			(void)e;
		}
		return true;
	}
	else if (is_len_required())
	{
		try
		{
			exec_len_required();
			if (fd_wait.size() == 0)
			{
				request = "";
				body = "";
			}
		}
		catch (NotReady &e)
		{
			(void)e;
		}
		return true;
	}
	else if (is_dir_listing(config["uri"]))
	{
		try
		{
			exec_dir_listing(config["uri"]);
			if (fd_wait.size() == 0)
			{
				request = "";
				body = "";
			}
		}
		catch (NotReady &e)
		{
			(void)e;
		}
		return true;
	}
	return false;
}


void Client::parse_request()
{
	std::map<std::string, std::string> config;
	std::string tmp;
	std::size_t start;
	std::size_t end;
	std::vector<std::string> f;


	if (is_bad_request())
	{
		try
		{
			exec_bad_request();
			if (fd_wait.size() == 0)
			{
				request = "";
				body = "";
			}
		}
		catch(NotReady &e)
		{
			(void)e;
		}
		return;
	}
	tmp = "";
	start = 0;
	end = 0;
	
	while(start < request.size())
	{
		end = request.find("\r\n",start);
		tmp = request.substr(start, end-start);
		if (start == 0)
		{
			f = split(tmp," ");
			config["method"] = f[0];
			config["uri"] = f[1];
			config["http_version"] = f[2];
		}
		else
		{
			f = split_sep(tmp,": ");
			if (f.size() >= 2)
			{
				config[f[0]] = f[1];
			}
		}
		f.clear();
		tmp = "";
		start = end + 2;
		end = 0;
	}
	if (other_traitment(config))
		return;

	try
	{
		Response a(config, *this);
		a.rp(socket);
	}
	catch(const NotReady& e)
	{
		(void)e;
	}
	
	if (fd_wait.size() == 0)
	{
		request = "";
		body = "";
		real_body = 0;
		size_body = 0;
		try
		{
			if(config.at("Connection") == "close")
				stat = -1;
		}
		catch(const std::out_of_range &e)
    	{
        	(void)e;
    	}
		
	}
}