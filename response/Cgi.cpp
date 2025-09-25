/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 12:23:35 by jerrandr          #+#    #+#             */
/*   Updated: 2025/09/25 14:58:48 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

Cgi::~Cgi()
{
	delete [] argv;
	delete utils;
	delete [] envp;
}

Cgi::Cgi(char **Envp, int length, Client &cl): Cl(cl)
{
	Config	cf;

	cf = Cl.getConfig();
	utils = new ExecUtils();
	argv = new char*[2];
	envp = Envp;
	lv = length;
	argv[0] = const_cast<char*>("/usr/bin/php-cgi");
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
	Pollfd				*pl = Cl.getPoll();
	int					status;
	std::stringstream	fl;

	fl << p;
	fl >> status;
	p = utils->getError(Cl, status);
	utils->SendResponse(pl, p, fdc);
}

void	Cgi::IfFound(std::string p, int fdc)
{
	Pollfd	*pl = Cl.getPoll();
	std::string	ext = getType(envp[4]);
	std::string	rp;
	rp = ParseCgi(p);
	rp = "HTTP/1.1 200 OK\r\nContent-Length: " + utils->ToString(rp.size()) + "\r\nContent-Type: " + ext + "\r\n\r\n" + rp;
	utils->SendResponse(pl, rp, fdc);
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

	res = utils->getExt(ct);
	if (res == ".php")
		res = ".html";
	res = Cl.getConfig().get_mime(res);
	return (res);	
}

void	Cgi::GetAndSend(int &fd, int fdc)
{
	std::string	p;
	std::string	st;
	Pollfd		*pl;

	pl = Cl.getPoll();
	if (!(pl->get_status(fd)  & POLLIN))
	{
		pl->add_new_fd(fd);
		pl->set_new_fd();
		throw NotReady();
	}
	if (pl->get_status(fd) & POLLIN)
		p = utils->getData(fd);
	else
		throw NotReady();
	st = getStatus(p);
	Cl.getPoll()->erase_fd(fd);
	pl->decrement_new_fd();
	if (st != "")
		IfNotFound(st, fdc);
	else
		IfFound(p, fdc);
}

void	Cgi::IfBody(Pollfd *pl, std::string body, int fd, int pid)
{
	if (body != "")
	{
		if (pl->get_status(fd) & POLLOUT)
		{
			write(fd, body.c_str(), body.size());
			pl->erase_fd(fd);
			pl->decrement_new_fd();
		}
		else
		{
			kill(pid, SIGTERM);
			throw NotReady();
		}
	}
}

void	Cgi::ParentTasks(Pollfd *pl, int fd2[2], int fd[2], int pid, int fdc)
{
	if ((pl->get_status(fd2[1])  & POLLIN))
	{
		pl->erase_fd(fd2[1]);
		pl->decrement_new_fd();
	}	
	close(fd2[0]);
	close(fd[1]);
	if (waitpid(pid, NULL, WNOHANG) == pid)
		GetAndSend(fd[0], fdc);
	else
	{
		Cl.pid = pid;
		Cl.fd_in = fd[0];
		Cl.fd_out = fd2[1];
		Cl.fl = true;
		throw NotReady();
	}
}

void	Cgi::IfNotActif(std::string body, int fdc, Pollfd *pl)
{
	int		fd[2];
	int		fd2[2];
	int		pid;

	Cl.bg = time(NULL);
	if (pipe(fd) < 0 || pipe(fd2) < 0)
		throw std::bad_alloc();
	if (!(pl->get_status(fd2[1])  & POLLIN))
	{
		pl->add_new_fd(fd2[1]);
		pl->set_new_fd();
		throw NotReady();
	}
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
		IfBody(pl, body, fd2[1], pid);
		ParentTasks(pl, fd2, fd, pid, fdc);
	}
}

void    Cgi::MyExec(int fdc, std::string body)
{
	Pollfd	*pl;

	Error504 = utils->getError(Cl, 504);
	pl = Cl.getPoll();
	if (!Cl.fl)
		IfNotActif(body, fdc, pl);
	else
	{
		int	res = waitpid(Cl.pid, NULL, WNOHANG); 
		if (res != Cl.pid)
		{
			if (utils->checkTimeOut(Cl.bg, time(NULL)))
			{
			
				kill(Cl.pid, SIGTERM);
				Cl.pid = false;
				utils->SendResponse(pl, Error504, fdc);
			}
			else
				throw NotReady();
		}
		else if (res == Cl.pid)
		{
			Cl.pid = false;
			GetAndSend(Cl.fd_in, fdc);
		}
	}
}
