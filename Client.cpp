/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:34:25 by msalohy           #+#    #+#             */
/*   Updated: 2025/10/02 19:02:24 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "response/Response.hpp"
Client::Client()
{
	socket = -1;
	request = "";
	status_connexion = 0;
	status_request = 0;
	size_body = 0;
	body = "";
	stat = 0;
	real_body = -1;
	request_time = 0;
	client_timeout = time(NULL);
	//+++++++++++++++++++
		pid = -1;
		fd_in = -1;
		fd_out = -1;
		fl = false;
		bg = 0;
	//++++++++++++++++++
}

Client::~Client() {}

Client::Client(const Client &other)
{
	*this = other;
}
Client &Client::operator=(const Client &other)
{
	socket = other.socket;
	request = other.request;
	status_connexion = other.status_connexion;
	status_request = other.status_request;
	size_body = other.size_body;
	body = other.body;
	stat = other.stat;
	real_body = other.real_body;
	config = other.config;
	polls = other.polls;
	request_time = other.request_time;
	client_timeout = other.client_timeout;
	//+++++++++++++++++++
	pid = other.pid;
	fd_in = other.fd_in;
	fd_out = other.fd_out;
	fl = other.fl;
	bg = other.bg;
	//++++++++++++++++++
	return (*this);
}

Client::Client(int s, Pollfd *poll, Config &conf)
{
	socket = s;
	request = "";
	reponse = "";
	status_connexion = 0;
	status_request = 0;
	size_body = 0;
	body = "";
	stat = 0;
	real_body = -1;
	config = conf;
	polls = poll;
	request_time = 0;
	client_timeout = time(NULL);
	//+++++++++++++++++++
		pid = -1;
		fd_in = -1;
		fd_out = -1;
		fl = false;
		bg = 0;
	//++++++++++++++++++
}
std::string Client::get_request() const
{
	return request;
}

int Client::get_socket_client() const
{
	return socket;
}

long Client::get_len_real_body()
{
	size_t size;
	std::string len;
	std::stringstream ss;

	size = 0;
	len = "";
	size = request.find("Content-Length:");
	if (size == std::string::npos)
		return -1;
	size = size + 16;
	for (size_t i = size; i < request.length(); i++)
	{
		if (request[i] == '\r' && i + 1 < request.length() && request[i + 1] == '\n')
			break;
		len += request[i];
	}
	ss << len;
	ss >> size;
	return (size);
}

size_t Client::get_len_body(std::string buffer)
{
	size_t start;

	start = 0;
	start = buffer.find("\r\n\r\n");
	if (start == std::string::npos)
		return 0;
	if (start + 4 < buffer.length())
		return 0;
	return (start + 4);
}

void Client::set_status_connexion(int status)
{
	status_connexion = status;
}



int Client::get_status() const
{
	return stat;
}

int Client::get_status_request()
{
	return status_request;
}
void Client::set_head(int size, std::string buffer)
{
	if (request != "" && request.find("\r\n\r\n") != std::string::npos)
	{
		return;
	}
	if (size == 0)
		return;
	else
	{
		for (int i = 0; i < size; i++)
		{
			request += buffer[i];
		}
	}
}
static std::string get_body(std::string buffer, int &size,std::string request)
{
	std::string body;
	std::vector<char> bo;
	size_t start;

	body = "";
	start = 0;
	if (buffer.find("\r\n\r\n") == std::string::npos && request.find("\r\n\r\n") == std::string::npos)
	{
		size = buffer.size();
		return "";
	}
	if (request.find("\r\n\r\n") == std::string::npos)
	{
		for (size_t j = 0; j < buffer.size(); j++)
		{
			if (j + 3 < buffer.size() && buffer[j] == '\r' && buffer[j + 1] == '\n' && buffer[j + 2] == '\r' && buffer[j + 3] == '\n')
			{
				start = j + 4;
				break;
			}
		}
	}
	size = start;
	while (start < buffer.size())
	{
		bo.push_back(buffer[start]);
		start++;
	}
	for (size_t i = 0; i < bo.size(); i++)
		body += bo[i];
	return body;
}



bool Client::is_chunked(std::string buffer)
{
	std::size_t size;
	std::string tmp;

	tmp = "";
	size = buffer.find("Transfer-Encoding: ");
	if (size == std::string::npos)
		return false;
	for (std::size_t i = size; i < buffer.size(); i++)
	{
		if (i + 1 < buffer.size() && buffer[i] == '\r' && buffer[i + 1] == '\n')
			break;
		tmp += buffer[i];
	}
	if (tmp == "Transfer-Encoding: chunked")
		return true;
	return false;
}


void Client::receve_message()
{

	char buffer[1024];
	int status;
	int size;

	size = 0;
	status_request = -1;
	std::memset(buffer, 0, sizeof(buffer));
	status = 0;
	status = recv(socket, buffer, 1024 - 1, 0);
	if (request_time == 0)
	{
		request_time = time(NULL);
	}

	if (status < 0)
		return;
	else if (status == 0)
	{
		stat = -1;
		return;
	}
	std::string tmp;

	tmp.append(buffer, status);
	body += get_body(tmp, size,this->request);
	size_body += status - size;
	set_head(size, tmp);
	if (real_body == -1)
		real_body = get_len_real_body();
	if (stat >= 1 || (is_chunked(request)))
	{
		stat = 1;
		stat = body_chunked(status, tmp);
		if (stat == 0)
		{
			body_unchunked();
			size_body = real_body;
			stat = 0;
			status_request = 1;
		}
	}
	else if (stat != 1)
	{
		if (real_body == size_body || (real_body == -1 && size_body == 0))
		{
			status_request = 1;
		}
		stat = 0;
	}
	if (request.find("\r\n\r\n") == std::string::npos)
		status_request = 2;
	if (real_body == 0 && !is_chunked(request) && is_post() && request.find("\r\n\r\n") != std::string::npos)
	{
		parse_request();
		if (fd_wait.size() == 0 &&  polls->get_new_fd_poll() <= 0 && !fl)
		{
			request = "";
			body = "";
			real_body = 0;
		}
		return ;
	}
	if (status_request == 1 && real_body == size_body && request.find("\r\n\r\n") != std::string::npos)
	{
		request_time = 0;
		status_request = 1;
		status = 2;
		size_body = 0;
		parse_request();
		if (fd_wait.size() == 0 && polls->get_new_fd_poll() <= 0)
		{
			request = "";
			body = "";
			real_body = 0;
		}
	}
}


void Client::set_request(std::string n)
{
	request = n;
}

void Client::verify_connex(int status)
{
	if (status == 1)
	{
		receve_message();
	}

}
std::size_t Client::size_fd_wait()
{
	return fd_wait.size();
}

Pollfd *Client::getPoll() const
{
	return (polls);
}

Config Client::getConfig() const
{
	return (config);
}

std::string Client::getBody() const
{
	return (body);
}

std::map<std::string, int> & Client::getFdWait()
{
	return (this->fd_wait);
}

ssize_t Client::get_timeout()
{
	return request_time;
}
void    Client::set_timeout(ssize_t t)
{
	request_time = t;
}
ssize_t Client::get_timeout_client()
{
	return client_timeout;
}

void    Client::set_status_client(int s)
{
	stat = s;
}
void    Client::set_socket(int s)
{
	socket  = s;
}