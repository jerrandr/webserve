/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 12:23:35 by jerrandr          #+#    #+#             */
/*   Updated: 2025/09/17 13:43:40 by jerrandr         ###   ########.fr       */
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
	utils = new ExecUtils(cf.get_errors());
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
	std::string	filename;
	Pollfd		*pl = Cl.getPoll();

	filename = "error/" + p + ".html";
	p = utils->getError(filename, Cl);
	utils->SendResponse(pl, p, fdc);
}

void	Cgi::IfFound(std::string p, int fdc)
{
	Pollfd	*pl = Cl.getPoll();
	std::string	ext = getType(envp[4]);
	std::cout << "P: {" << p<< "}\n";
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

	res = "";
	if (ct.find("=") != std::string::npos)	
		res = ct.substr(ct.find("="), ct.length());
	if (res.find(".") != std::string::npos)
		res = res.substr(res.find("."), res.length());
	if (res == ".php")
		res = ".html";
	res = Cl.getConfig().get_mime(res);
	return (res);	
}

void	Cgi::MyExec2(int &fd, int fdc)
{
	std::string	p;
	std::string	st;

	Cl.getPoll()->add_new_fd(fd);
	p = utils->getData(fd);
	std::cout << RED << "P: {" << p << "}\n" << R;
	st = getStatus(p);
	Cl.getPoll()->erase_fd(fd);
	if (st != "")
		IfNotFound(st, fdc);
	else
		IfFound(p, fdc);
}

void    Cgi::MyExec(int fdc, std::string body)
{
	int		fd[2];
	int		fd2[2];
	int		pid;
	time_t	bg;
	Pollfd	*pl;

	Error504 = utils->getError("error/504.html", Cl);
	pl = Cl.getPoll();
	bg = time(NULL);
	if (pipe(fd) < 0 || pipe(fd2) < 0)
		exit(0);
	pl->add_new_fd(fd2[1]);
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
		{
			std::cout << "BODY: {" << body << "}" << std::endl;
			if (pl->get_status(fd2[1]) & POLLIN)
			{
				write(fd2[1], body.c_str(), body.size());
				pl->erase_fd(fd2[1]);
			}
		}
		close(fd2[0]);
		pl->erase_fd(fd2[1]);
		close(fd[1]);
		while (true)
		{
			if (waitpid(pid, NULL, WNOHANG) == pid)
			{
				MyExec2(fd[0], fdc);
				break;
			}
			if (utils->checkTimeOut(bg, time(NULL)))
			{
				std::cout << RED << "TIMEOUT" << R << std::endl;
				kill(pid, SIGTERM);
				std::cout << "ERROR {" << Error504 << "}\n";
				utils->SendResponse(Cl.getPoll(), Error504, fdc);
				break;
			}
			sleep(1);
		}
	}
}