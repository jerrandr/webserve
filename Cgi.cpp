/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 12:23:35 by jerrandr          #+#    #+#             */
/*   Updated: 2025/07/22 10:37:58 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

Cgi::~Cgi() {}

Cgi::Cgi(char **Envp, int length, Pollfd *p)
{
	argv = new char*[2];
	CgiName = new char;

	envp = Envp;
	pl = p;
	lv = length;

	CgiName = const_cast<char*>("/usr/bin/php-cgi");
	argv[0] = CgiName;
	argv[1] = NULL;
}


// void	Cgi::sendImg(std::string path, int socket)
// {
// 	std::cout << "PATH: |" << path << "|" << std::endl;
// 	std::ifstream img(path.c_str(), std::ios::binary);

// 	if (img.fail())
// 	{
// 		perror("IMG MISY BLEM");
// 		exit(0);
// 	}

// 	std::stringstream tmp;

// 	tmp << img.rdbuf();
// 	std::string rp;
// 	std::string h;
// 	size_t		lgth;

// 	rp = tmp.str();
// 	lgth = rp.size();
// 	h = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(lgth)  + "\r\nContent-Type: image/png\r\n\r\n";
// 	send(socket, h.c_str(), h.size(), 0);
// 	send(socket, rp.c_str(), rp.size(), 0);
// }

// void	Cgi::sendPdf(std::string path, int socket)
// {
// 	std::cout << "PATH: |" << path << "|" << std::endl;
// 	std::ifstream img(path.c_str(), std::ios::binary);

// 	if (img.fail())
// 	{
// 		perror("IMG MISY BLEM");
// 		exit(0);
// 	}

// 	std::stringstream tmp;

// 	tmp << img.rdbuf();
// 	std::string rp;
// 	std::string h;
// 	size_t		lgth;

// 	rp = tmp.str();
// 	lgth = rp.size();
// 	h = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(lgth)  + "\r\nContent-Type: applications/pdf\r\n\r\n";
// 	send(socket, h.c_str(), h.size(), 0);
// 	send(socket, rp.c_str(), rp.size(), 0);
// }

std::string	Cgi::getStatus(std::string p)
{
	std::string res;

	res = "";
	if (p.find("status") != std::string::npos)
	{
		res = p.substr(p.find("status"), p.length());
		std::cout << "RES: " << res << std::endl;
	}
	return (res);
}

void	Cgi::IfNotFound(std::string p, int fdc)
{
	std::string			nbr;
	struct stat			st;
	stat("error/404.html", &st);
	p = ParseCgi(p);
	nbr = utils.ToString(st.st_size);
	p = "HTTP/1.1 404 not found\r\nContent-Length: " + nbr + "\r\nContent-Type: text/html\r\n\r\n" + p;
	if (pl->get_status(fdc) & POLLIN)
	{
		if (send(fdc, p.c_str(), p.size(), 0) < 0)
		{
			std::cout << "EXECVE ERROR" << std::endl;
			exit(0);
		}
	}
}

void	Cgi::IfFound(std::string p, int fdc)
{
	p = ParseCgi(p);
	p = "HTTP/1.1 200 OK\r\nContent-Length: " + utils.ToString(p.size()) + "\r\nContent-Type: text/html\r\n\r\n" + p;
	if (pl->get_status(fdc) & POLLIN)
	{
		if (send(fdc, p.c_str(), p.size(), 0) < 0)
		{
			std::cout << "EXECVE ERROR" << std::endl;
			exit(0);
		}
	}
}

std::string	Cgi::ParseCgi(std::string content)
{
	std::string res;

	if (content.find("\r\n") != std::string::npos)
	{
		res = content.substr(content.find("\r\n"), content.length());
		return (res);
	}
	return (content);
}

void	Cgi::MyExec2(int fd[2], int fdc)
{
	std::string p;

	p = utils.getData(fd[0]);
	close(fd[1]);
	close(fd[0]);
	getStatus(p);
	std::cout << "P: " << p << std::endl;
	if (p.find("404 not found") != std::string::npos)
		IfNotFound(p, fdc);
	else
		IfFound(p, fdc);
}

void    Cgi::MyExec(int fdc, std::string body)
{
	int fd[2];
	int fd2[2];
	int pid;

	std::cout << "BODY_CGI: " << body << std::endl;
	if (pipe(fd) < 0 || pipe(fd2) < 0)
		exit(0);
	pid = fork();
	if (pid == 0)
	{
		dup2(fd2[0], STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		execve("/usr/bin/php-cgi", argv, envp);
		exit(0);
	}
	else
	{
		if (body != "")
			write(fd2[1], body.c_str(), body.size());
		close(fd2[0]);
		close(fd2[1]);
		waitpid(pid, NULL, 0);
		close(fd[1]);
		MyExec2(fd, fdc);
	}
}