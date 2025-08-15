/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_processing.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 08:48:38 by msalohy           #+#    #+#             */
/*   Updated: 2025/08/15 14:55:27 by jerrandr         ###   ########.fr       */
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
		// path = this->config.get_errors().get_path_500();
		// if (access(path.c_str(), F_OK | R_OK) < 0)
		// {
			t = "413 PLAYLOAD TOO LARGE";
			ss << t.size();
			t = "HTTP/1.1 500 KO\r\nContent-Length: " + ss.str() + "\r\nContent-Type: text/html\r\n\r\n" + t;
			if ((this->polls->get_status(socket) & POLLOUT) && !(this->polls->get_status(socket) & POLLHUP))
				send(socket, t.c_str(), t.size(), 0);
			return;
		// }
		// fd = fd_is_ready(path, polls, fd_wait);
		// if (fd != -1)
		// {
		// 	t = get_html_page(fd);
		// 	ss << t.size();
		// 	t = "HTTP/1.1 500 KO\r\nContent-Length: " + ss.str() + "\r\nContent-Type: text/html\r\n\r\n" + t;
		// 	if ((this->polls->get_status(socket) & POLLOUT) && !(this->polls->get_status(socket) & POLLHUP))
		// 		send(socket, t.c_str(), t.size(), 0);
		// 	fd_closed(fd, polls, fd_wait, path);
		// }
		// return;
	}
	fd = fd_is_ready(path, polls, fd_wait);
	if (fd != -1)
	{
		t = get_html_page(fd);
		ss << t.size();
		t = "HTTP/1.1 413 KO\r\nContent-Length: " + ss.str() + "\r\nContent-Type: text/html\r\n\r\n" + t;
		if ((this->polls->get_status(socket) & POLLOUT) && !(this->polls->get_status(socket) & POLLHUP))
			send(socket, t.c_str(), t.size(), 0);
		fd_closed(fd, polls, fd_wait, path);
	}
}
void Client::exec_dir_listing(std::string uri)
{
	Location loc;
	std::string path;

	loc = config.get_location_match(uri);
	path = config.get_real_path(uri, loc);
	directory_listing(path);
}
int Client::is_dir_listing(std::string uri)
{
	Location loc;
	std::string path;

	loc = config.get_location_match(uri);
	path = config.get_real_path(uri, loc);
	std::cout << "path = " << path << std::endl;
	if (is_directory(path))
	{
		if (loc.get_directory_listing() && loc.get_index() == "")
			return (1);
	}
	return (0);
}


bool    Client::other_traitment(std::map<std::string, std::string> config)
{
	if (this->config.get_max_allowed_size() < this->get_len_real_body())
	{
		std::cout << "max body size" << std::endl;
		max_body_size_trait();
		if (fd_wait.size() == 0)
		{
			requette = "";
			body = "";
		}
		return true;
	}
	else if (is_dir_listing(config["uri"]))
	{
		std::cout << "is dir listing" << std::endl;
		try
		{
			exec_dir_listing(config["uri"]);
			if (fd_wait.size() == 0)
			{
				requette = "";
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


void Client::parse_requette()
{
	std::map<std::string, std::string> config;
	std::string temp1;
	std::string temp2;

	temp1 = "";
	temp2 = "";
	// std::cout <<"{"<<requette<<"}"<< std::endl;
	if (is_bad_request())
	{
		try
		{
			exec_bad_request();
			std::cout << "fd_wait " << fd_wait.size() << std::endl;
			if (fd_wait.size() == 0)
			{
				requette = "";
				body = "";
				std::cout << "vita" << std::endl;
			}
		}
		catch(NotReady &e)
		{
			std::cout << "non " <<e.what()<<std::endl;
			// (void)e;
		}
		std::cout << "bad request" << std::endl;
		return;
	}
	for (size_t i = 0; i < requette.size(); i++)
	{
		if (requette[i] == ' ' && ((temp1 == "GET") || (temp1 == "HEAD") || (temp1 == "OPTIONS") || (temp1 == "TRACE") || (temp1 == "PUT") || (temp1 == "DELETE") || (temp1 == "POST") || (temp1 == "PATCH") || (temp1 == "CONNECT")))
		{
			config["method"] = temp1;
			temp1 = "";
			i += 1;
		}
		else if (requette[i] == ' ' && (temp1.find("/") != std::string::npos && temp1 != "HTTP/1.1"))
		{
			config["uri"] = temp1;
			temp1 = "";
			i += 1;
		}
		else if (temp1.find("HTTP/1.1") != std::string::npos)
		{
			config["http_version"] = temp1;
			temp1 = "";
			i += 2;
		}
		else if (requette[i] == ':' && i + 1 < requette.size() && requette[i + 1] == ' ')
		{
			i += 2;
			for (size_t j = i; j < requette.size(); j++)
			{
				if (j + 1 < requette.size() && requette[j] == '\r' && requette[j + 1] == '\n')
				{
					i += 2;
					break;
				}
				temp2 += requette[j];
			}
			i += temp2.size();
			if (temp1 != " " && temp2 != " ")
			{
				// std::cout <<"{"<<temp1 <<"}{"<< temp2 <<"}"<<std::endl;
				config[temp1] = temp2;
				temp2 = "";
			}
			temp1 = "";
			if (i >= requette.size())
				return;
		}
		temp1 += requette[i];
	}
	// std::cout << requette << std::endl;
	// std::cout <<"{Methode}" << "{" << config["method"] <<"}"<<std::endl;
	// std::cout <<"{uri}" << "{" << config["uri"] <<"}"<<std::endl;
	// std::cout <<"{http_version}" << "{" << config["http_version"] <<"}"<<std::endl;
	// if(config["method"] == "POST")
	// {
	//         std::ofstream fd("test.out");
	//         std::cout << body << std::endl;
	//         fd << body;
	// }
	std::cout << "link = " << config["uri"] << std::endl;
	if (other_traitment(config))
		return;
	// config["uri"] = this->config.get_real_path(config["uri"],this->config.get_location_match(config["uri"]));
	std::cout << "tsy dir listing" << std::endl;
	try
	{
		Requette a(config, *this);
		a.rp(socket);
	}
	catch(const NotReady& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	if (fd_wait.size() == 0)
	{
		requette = "";
		body = "";
		real_body = 0;
		size_body = 0;
	}
}