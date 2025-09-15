/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExecUtils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 12:25:49 by jerrandr          #+#    #+#             */
/*   Updated: 2025/09/15 14:04:09 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTILS_HPP
#define EXECUTILS_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>
#include <vector>
#include <map>
#include <algorithm>
#include "../Pollfd.hpp"
#include "../utils.h"
#include "../NotReady.hpp"
#include "../Client.hpp"

class Client;
class ExecUtils
{
	private:
		std::string				getErrorUtils(std::string st,  Client & cl);
		// std::string	getErrorUtils(std::string st,  Pollfd *polls, std::map<std::string, int> &fd_wait);	
		std::string	getStatus(std::string fl);
		std::map<std::string, std::string> Er;
		std::string	Erro;
	public:
		ExecUtils();
		~ExecUtils();
		ExecUtils &operator=(ExecUtils const & cpy) {(void)cpy;return(*this);};
		ExecUtils(ExecUtils const & cpy) {(void)cpy;};
		std::string			ToString(int nbr);
		std::string 		getData(std::string filename, Client & cl, int &fl);
		// std::string			getData(std::string filename, Pollfd *polls, std::map<std::string, int> &fd_wait, int &fl);
		// std::string			getError(std::string filename, Pollfd *polls, std::map<std::string, int> &fd_wait);
		std::string			getError(std::string filename, Client &cl);
		std::string			getData(int fd);
		bool				checkTimeOut(time_t begin, time_t end);
		std::string			getExt(std::string filename);
		std::string			CheckError(std::string	rp, Client &Cl);
		void				SendResponse(Pollfd *pl, std::string rp, int fdc);
};
							

#endif