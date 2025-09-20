/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 12:26:47 by jerrandr          #+#    #+#             */
/*   Updated: 2025/09/20 09:54:20 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "BodyUpload.hpp"
#include "ExecUtils.hpp"
#include "Cgi.hpp"
#include <cstdio>
#include "../Client.hpp"

class Pollfd;
class Cgi;
class Client;
class ExecUtils;

class Response
{
	private:
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
		void					Delete(std::string path);
		int						IfDelete(int socket);
        int	             		IfDirList(Location lt);
		bool					ifCgi2(Location Loc);
	public:
				Response(std::map<std::string, std::string> config, Client  &cl);
				~Response();
		void	rp(int socket);
};

#endif