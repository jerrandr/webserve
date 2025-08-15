/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Requette.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 12:26:47 by jerrandr          #+#    #+#             */
/*   Updated: 2025/08/13 08:29:29 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUETTE_HPP
#define REQUETTE_HPP

#include "BodyUpload.hpp"
#include "ExecUtils.hpp"
#include "Cgi.hpp"
#include <cstdio>
#include "Client.hpp"

class Pollfd;
class Cgi;
class Client;

class Requette
{
	private:
		ExecUtils							utils;
		Cgi									*cgi;
		Pollfd								*pl;
		std::map<std::string, std::string>	rq;
		std::vector<Location>				lc;
		std::string							body;
		std::string							ctType;
		char								**envp;
		int									lv;
		Client								&Cl;
		
		void				initEnvp(std::string rt, std::string st, std::string bd);
							Requette(Requette const &cpy);
		Requette			&operator=(Requette const &cpy);
		int					findLoc2(std::vector<std::string> UriLoc, std::vector<std::string> toFind);
		Location			findLoc();

		void				redir_rp(std::string redir, int socket);
		std::string			redir_rp2(std::string redir);
		void				ifCgi(Location Loc, int socket, std::string bd);
		void				rp2(int socket);
		void				rp3(int socket);
		std::stringstream	getData(std::string filename);
		std::string			ToString(int nbr);
		int					IfDelete(int socket);

	public:
		Requette(std::map<std::string, std::string> config, Client  &cl);
		void rp(int socket);
		~Requette();
};

#endif