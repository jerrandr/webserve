/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 12:23:35 by jerrandr          #+#    #+#             */
/*   Updated: 2025/08/14 13:41:43 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

Cgi::~Cgi() {}

Cgi::Cgi(char **Envp, int length, Client &cl): Cl(cl)
{
	argv = new char*[2];
	CgiName = new char;

	envp = Envp;
	lv = length;
	CgiName = const_cast<char*>("/usr/bin/php-cgi");
	argv[0] = CgiName;
	argv[1] = NULL;
}

std::string	Cgi::getStatus(std::string p)
{
	std::string res;

	res = "";
	if (p.find("Status") != std::string::npos)
	{
		res = p.substr(8, p.length());
		res = res.substr(0, res.find(" "));
	}
	return (res);
}

void	Cgi::IfNotFound(std::string p, int fdc)
{
	std::string	filename;
	Pollfd		*pl = Cl.getPoll();

	filename = "error/" + p + ".html";
	p = utils.getError(filename, Cl.getPoll(), Cl.getFdWait());
	if ((pl->get_status(fdc) & POLLOUT) && !(pl->get_status(fdc) & POLLHUP))
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
	Pollfd	*pl = Cl.getPoll();
	std::string	ext = getType(envp[5]);
	std::cout << "P: {" << p<< "}\n";
	std::string	rp;
	rp = ParseCgi(p);
	rp = "HTTP/1.1 200 OK\r\nContent-Length: " + utils.ToString(rp.size()) + "\r\nContent-Type: " + ext + "\r\n\r\n" + rp;
	if ((pl->get_status(fdc) & POLLOUT) && !(pl->get_status(fdc) & POLLHUP))
	{
		if (send(fdc, rp.c_str(), rp.size(), 0) < 0)
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
		res = content.substr(content.find("\r\n") + 4, content.length());
		return (res);
	}
	return (content);
}

std::string	Cgi::getType(std::string ct)
{
	std::string	res;

	res = "";
	if (ct.find("=") != std::string::npos)	
		res = ct.substr(ct.find("="), ct.length());
	if (res.find(".") != std::string::npos)
		res = res.substr(res.find("."), res.length());
	res = Cl.getConfig().get_mime(res);
	return (res);	
}

void	Cgi::MyExec2(int &fd, int fdc)
{
	std::string	p;
	std::string	st;

	Cl.getPoll()->add_new_fd(fd);
	p = utils.getData(fd);
	st = getStatus(p);
	Cl.getPoll()->erase_fd(fd);
	if (st != "")
		IfNotFound(st, fdc);
	else
		IfFound(p, fdc);
}

void    Cgi::MyExec(int fdc, std::string body)
{
	int fd[2];
	int fd2[2];
	int pid;

	if (pipe(fd) < 0 || pipe(fd2) < 0)
		exit(0);
	pid = fork();
	if (pid == 0)
	{
		dup2(fd2[0], STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		execve("/usr/bin/php-cgi", argv, envp);
		perror("execve");
		exit(0);
	}
	else
	{
		if (body != "")
		{
			std::cout << "BODY: {" << body << "}" << std::endl;
			write(fd2[1], body.c_str(), body.size());
		}
		close(fd2[0]);
		close(fd2[1]);
		close(fd[1]);
		MyExec2(fd[0], fdc);
		waitpid(pid, NULL, 0);
	}
}