/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 12:23:35 by jerrandr          #+#    #+#             */
/*   Updated: 2025/10/13 13:10:09 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"
#include "../signal_handling/SignalHandling.hpp"

Cgi::~Cgi()
{
	delete [] argv;
	delete utils;
	delete [] envp;
}

Cgi::Cgi(char **Envp, int length, Client &cl, std::string path, std::string	query): Cl(cl)
{
	Config	cf;

	qr = query;
	pth = path;
	cf = Cl.getConfig();
	utils = new ExecUtils();
	argv = new char*[2];
	envp = Envp;
	lv = length;
	argv[0] = const_cast<char*>(pth.c_str());
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
	int					status;
	std::stringstream	fl;

	fl << p;
	fl >> status;
	p = utils->getError(Cl, status);
	utils->SendResponse(Cl, p, fdc);
}

void	Cgi::IfFound(std::string p, int fdc)
{
	std::string	ext = getType(envp[4]);
	std::string	rp;
	rp = ParseCgi(p);
	rp = "HTTP/1.1 200 OK\r\nContent-Length: " + utils->ToString(rp.size()) + "\r\nContent-Type: " + ext + "\r\n\r\n" + rp;
	utils->SendResponse(Cl, rp, fdc);
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
		Cl.len_inc();
		throw NotReady();
	}
	if (pl->get_status(fd) & POLLIN)
		p = utils->getData(fd);
	else
		throw NotReady();
	st = getStatus(p);
	close(fd);
	Cl.getPoll()->erase_fd(fd);
	fd = -1;
	pl->decrement_new_fd();
	Cl.len_dec();
	if (st != "")
		IfNotFound(st, fdc);
	else
		IfFound(p, fdc);
}

void		Cgi::IfBody(Pollfd *pl, std::string body, int fd2[2], int pid)
{
	std::string	tmp;

	tmp = envp[1];
	if (body != "")
	{
		if (pl->get_status(fd2[1]) & POLLOUT)
		{
			write(fd2[1], body.c_str(), body.size());
			if (tmp.find("POST") != std::string::npos && qr != "")
				write(fd2[1], qr.c_str(), qr.size());			
			pl->erase_fd(fd2[1]);
			pl->decrement_new_fd();
			Cl.len_dec();
		}
		else
		{
			kill(pid, SIGTERM);
			throw NotReady();
		}
	}
	close(fd2[1]);
}

void	Cgi::ParentTasks(Pollfd *pl, int fd2[2], int fd[2], int pid, int fdc)
{
	if ((pl->get_status(fd2[1])  & POLLIN))
	{
		pl->erase_fd(fd2[1]);
		pl->decrement_new_fd();
		Cl.len_dec();
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

void		Cgi::IfNotActif(std::string body, int fdc, Pollfd *pl)
{
	int		fd[2];
	int		fd2[2];
	int		pid;

	Cl.bg = time(NULL);
	if (pipe(fd) < 0 )
		throw std::bad_alloc();
	if (pipe(fd2) < 0)
		throw std::bad_alloc();
	
	if (!(pl->get_status(fd2[1])  & POLLIN))
	{
		pl->add_new_fd(fd2[1]);
		pl->set_new_fd();
		Cl.len_inc();
		throw NotReady();
	}
	pid = fork();
	if (pid == 0)
	{
		dup2(fd2[0], STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		execve(pth.c_str(), argv, envp);
		close(fd2[1]);
		close(fd[1]);
		close(fd2[0]);
		close(fd[0]);
		throw SignalHandling::ExceptSTop();
	}
	else
	{
		IfBody(pl, body, fd2, pid);
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
		if (res == 0)
		{
			if (utils->checkTimeOut(Cl.bg, time(NULL)))
			{
				if (Cl.pid != 0)
					kill(Cl.pid, SIGTERM);
				Cl.pid = false;
				close(Cl.fd_in);
				Cl.fd_in = -1;
				Cl.fd_out = -1;
				utils->SendResponse(Cl, Error504, fdc);
			}
			else
				throw NotReady();
		}
		else if (res == Cl.pid)
		{
			Cl.pid = -1;
			Cl.fl = false;
			Cl.fd_out = -1;
			GetAndSend(Cl.fd_in, fdc);
		}
	}
}
