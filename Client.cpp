/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 14:34:25 by msalohy           #+#    #+#             */
/*   Updated: 2025/09/11 09:02:37 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
	socket = -1;
	requette = "";
	status_connexion = 0;
	status_requette = 0;
	size_body = 0;
	body = "";
	stat = 0;
	real_body = 0;
	request_time = 0;
	client_timeout = time(NULL);
}

Client::~Client() {}

Client::Client(const Client &other)
{
	*this = other;
}
Client &Client::operator=(const Client &other)
{
	socket = other.socket;
	requette = other.requette;
	status_connexion = other.status_connexion;
	status_requette = other.status_requette;
	size_body = other.size_body;
	body = other.body;
	stat = other.stat;
	real_body = other.real_body;
	config = other.config;
	polls = other.polls;
	request_time = other.request_time;
	client_timeout = other.client_timeout;
	return (*this);
}

Client::Client(int s, Pollfd *poll, Config &conf)
{
	socket = s;
	requette = "";
	reponse = "";
	status_connexion = 0;
	status_requette = 0;
	size_body = 0;
	body = "";
	stat = 0;
	real_body = 0;
	config = conf;
	polls = poll;
	request_time = 0;
	client_timeout = time(NULL);
}
std::string Client::get_requette() const
{
	return requette;
}

int Client::get_socket_client() const
{
	return socket;
}

size_t Client::get_len_real_body()
{
	size_t size;
	std::string len;
	std::stringstream ss;

	size = 0;
	len = "";
	size = requette.find("Content-Length:");
	if (size == std::string::npos)
		return 0;
	size = size + 16;
	for (size_t i = size; i < requette.length(); i++)
	{
		if (requette[i] == '\r' && i + 1 < requette.length() && requette[i + 1] == '\n')
			break;
		len += requette[i];
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

void Client::send_message()
{
	if (reponse == "")
	{
		std::string test;
		std::string html;
		std::ifstream fd("index.html");
		std::stringstream ss;
		int size;

		test = "";
		if (fd.fail())
			std::perror("Error");
		size = 0;
		while (std::getline(fd, html))
		{
			size += html.length();
			test += html;
		}
		ss << size;
		// test = "HTTP/1.1 200 OK\r\nContent-Length: "+ss.str()+"\r\nContent-Type: text/html\r\n\r\n"+ test;
		size = send(socket, test.c_str(), strlen(test.c_str()), 0);
		if (size == -1)
			std::perror("error 1");
	}
	else
	{
		std::string test;
		int size;
		std::stringstream ss;

		size = 0;
		size += size_body;
		std::cout << "size body " << size_body << " " << reponse.size() << std::endl;
		ss << size_body;
		// test = "HTTP/1.1 200 OK\r\nContent-Length: "+ss.str()+"\r\nContent-Type: image/png\r\n\r\n";
		size += test.size();
		test.append(reponse, size_body);
		size += size_body;
		size = send(socket, &test[0], size, 0);
		if (size == -1)
			std::perror("error 1");
	}
	status_requette = 0;
}

int Client::get_status() const
{
	return stat;
}

int Client::get_status_requette()
{
	return status_requette;
}
void Client::set_head(int size, std::string buffer)
{
	if (requette != "" && requette.find("\r\n\r\n") != std::string::npos)
	{
		return;
	}
	if (size == 0)
		return;
	else
	{
		for (int i = 0; i < size; i++)
		{
			requette += buffer[i];
		}
	}
}
static std::string get_body(std::string buffer, int &size,std::string requette)
{
	std::string body;
	std::vector<char> bo;
	size_t start;

	body = "";
	start = 0;
	if (buffer.find("\r\n\r\n") == std::string::npos && requette.find("\r\n\r\n") == std::string::npos)
	{
		size = buffer.size();
		return "";
	}
	if (requette.find("\r\n\r\n") == std::string::npos)
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

// std::vector<std::string>     Client::body_split()
// {
//         std::string new_body;
//         std::vector<std::string> spl;
//         std::vector <std::string> r;

//         new_body = "";
//         std::ofstream fd("test.out");

//         fd << requette +body;
//         spl = split(body,"\r\n");
//         if(spl.size() >= 5)
//         {
//                 for(size_t i = 3 ; i < spl.size(); i++)
//                 {
//                         if(i < spl.size()-1)
//                                 new_body += spl[i];
//                 }
//         }
//         else
//         {
//                  for(size_t i = 3 ; i < spl.size(); i++)
//                 {
//                         new_body += spl[i];
//                 }
//         }
//         if(new_body != "")
//                 body = new_body;
//         for(size_t i = 1; i < 3; i++)
//         {
//                 if(i < spl.size())
//                         r.push_back(spl[i]);
//         }
//         return r;
// }

static bool is_chunked(std::string buffer)
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
	// static int u = 0;
	char buffer[1024];
	int status;
	int size;

	size = 0;
	status_requette = -1;
	std::memset(buffer, 0, sizeof(buffer));
	status = 0;
	status = recv(socket, buffer, 1024 - 1, 0);
	if (request_time == 0)
	{
		request_time = time(NULL);
	}
	// u += status;
	// std::cout << buffer << std::endl;
	if (status < 0)
		return;
	else if (status == 0)
	{
		// std::cout << "Client deconnected" << std::endl;
		stat = -1;
		return;
	}
	std::string tmp;

	tmp.append(buffer, status);
	// std::cout << u << std::endl;
	// if (u == 2494804)
	// std::cout << buffer << std::endl;
	body += get_body(tmp, size,this->requette);
	size_body += status - size;
	// std::cout << "{" << body << "}" << std::endl;
	// std::cout << "misy anle header " <<size<<std::endl;
	set_head(size, tmp);
	std::cout <<"{"<<buffer <<"}"<<std::endl;
	if (real_body == 0)
		real_body = get_len_real_body();
	if (stat >= 1 || (is_chunked(tmp)))
	{
		stat = 1;
		// std::cout << "chunked" <<std::endl;
		stat = body_chunked(status, tmp);
		if (stat == 0)
		{
			std::cout << real_body << " " << size_body << std::endl;
			// std::cout << "dude" << std::endl;
			// std::cout << "{" << body << "}" << std::endl;
			// std::cout << requette<< std::endl;
			// std::cout << "end" << std::endl;
			body_unchunked();
			// std::ofstream fd("test.out");
			std::cout << body << std::endl;
			// fd << body ;
			size_body = real_body;
			stat = 0;
			status_requette = 1;
		}
	}
	else if (stat != 1)
	{
		std::cout << real_body << "   " << size_body << std::endl;
		if (real_body == size_body)
		{
			status_requette = 1;
		}
		stat = 0;
	}
	if (requette.find("\r\n\r\n") == std::string::npos)
		status_requette = 2;
	if (status_requette == 1 && real_body == size_body && requette.find("\r\n\r\n") != std::string::npos)
	{
		request_time = 0;
		status_requette = 1;
		status = 2;
		size_body = 0;
		// requette += body;
		// std::cout << "real_" << real_body << " " << size_body << std::endl;

		// std::cout << "------------------message--------------------" << std::endl;
		// std::cout  << body <<std::endl;
		// std::cout << "----------------------------------------------" << std::endl;
		// std::cout << "start parse()" << std::endl;
		parse_requette();
		if (fd_wait.size() == 0)
		{
			requette = "";
			body = "";
			real_body = 0;
		}
	}
}


void Client::set_requette(std::string n)
{
	requette = n;
}

void Client::verify_connex(int status)
{
	if (status == 1)
	{
		receve_message();
	}
	else if (status == 2 && status_requette == 1 && reponse != "" && fd_wait.size() == 0)
	{
		send_message();
		size_body = 0;
		real_body = 0;
	}
}
std::size_t Client::size_fd_wait()
{
	return fd_wait.size();
}
// JERRY MODIF
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