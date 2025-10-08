/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 12:25:36 by jerrandr          #+#    #+#             */
/*   Updated: 2025/10/07 21:02:18 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CGI_HPP
#define CGI_HPP

#include "ExecUtils.hpp"
#include "../Client.hpp"
#include "../Socket.hpp"
#include "../Server.hpp"
#include <sys/wait.h>
#include "../Pollfd.hpp"
#include <iostream>
#include <sys/stat.h>
#include "../Client.hpp"

#define RED "\033[31m"
#define R "\033[0m"
class Client;
class ExecUtils;

class Cgi
{
    private:
		std::string	Error504;
		ExecUtils	*utils;
		char		**envp;
		char		**argv;
		int			lv;
		Client		&Cl;
		std::string	pth;
		Cgi			&operator=(Cgi const & cpy);
					Cgi(Cgi const & cpy);
		std::string	getStatus(std::string p);
		void		IfNotFound(std::string p, int fdc);
		void		IfFound(std::string p, int fdc);
		void		GetAndSend(int &fd, int fdc);
		void		IfNotActif(std::string body, int fdc, Pollfd *pl);
		void		IfBody(Pollfd *pl, std::string body, int fd2[2], int pid);
		void		ParentTasks(Pollfd *pl, int fd2[2], int fd[2], int pid, int fdc);
		std::string	ParseCgi(std::string content);
		std::string	getType(std::string ct);
	public:
				~Cgi();
				Cgi(char **Envp, int length, Client &cl, std::string path);
		void	MyExec(int fdc, std::string body);
};

#endif