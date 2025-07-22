/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 12:25:36 by jerrandr          #+#    #+#             */
/*   Updated: 2025/07/22 08:32:09 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CGI_HPP
#define CGI_HPP

#include "ExecUtils.hpp"
#include "Client.hpp"
#include "Socket.hpp"
#include "Server.hpp"
#include <sys/wait.h>
#include "Pollfd.hpp"
#include <iostream>
#include <sys/stat.h>

#define RED "\033[31m"
#define R "\033[0m"

class Cgi
{
    private:
		ExecUtils	utils;
		char		**envp;
		char		**argv;
		char		*CgiName;
		Pollfd		*pl;
		int			lv;
		
		Cgi					&operator=(Cgi const & cpy);
							Cgi(Cgi const & cpy);
		std::stringstream	getData(std::string filename);
		std::string			getStatus(std::string p);
		// void				sendImg(std::string path, int socket);
		// void				sendPdf(std::string path, int socket);
		void				initEnvp(std::string rt);
		void				IfNotFound(std::string p, int fdc);
		void				IfFound(std::string p, int fdc);
		void				MyExec2(int fd[2], int fdc);
		std::string			ParseCgi(std::string content);

	public:
				~Cgi();	
				Cgi(char **Envp, int length, Pollfd *p);
		void	MyExec(int fdc, std::string body);
};

#endif