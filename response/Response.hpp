/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 12:26:47 by jerrandr          #+#    #+#             */
/*   Updated: 2025/10/07 11:06:21 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "BodyUpload.hpp"
#include "ExecUtils.hpp"
#include "Cgi.hpp"
#include <cstdio>
#include "../Client.hpp"
#include "Send.hpp"

class Pollfd;
class Cgi;
class Client;
class ExecUtils;

class Response
{
	private:
		std::string							error_403;
		ExecUtils							*utils;
		Cgi									*cgi;
		Pollfd								*pl;
		std::map<std::string, std::string>	rq;
		std::vector<Location>				lc;
		std::string							ctType;
		char								**envp;
		int									lv;
		Client								&Cl;
		std::vector<std::string>			envStock;

								Response(Response const &cpy);
		void					initEnvp(std::string rt, std::string bd);
		Response				&operator=(Response const &cpy);
		int						findLoc2(std::vector<std::string> UriLoc, std::vector<std::string> toFind);
		Location				findLoc();
		void					redir_rp(std::string redir, int socket);
		std::string				redir_rp2(std::string redir);
		void					ifCgi(Location Loc, int socket, std::string bd);
		void					rp2(int socket, Location &Loc);
		void					rp3(int socket, Location Loc);
		std::string				rp5(std::string	rt);
		void					Delete(std::string path, std::string &rp);
		int						IfDelete(int socket, Location loc);
        int	             		IfDirList(Location lt);
		bool					ifCgi2(Location Loc);
		bool					Check405_501(Location Loc, int socket);
		bool					Check_502(std::string str);
	public:
				Response(std::map<std::string, std::string> config, Client  &cl);
				~Response();
		void	rp(int socket);
};

#endif