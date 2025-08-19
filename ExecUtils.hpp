/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExecUtils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 12:25:49 by jerrandr          #+#    #+#             */
/*   Updated: 2025/08/15 14:50:47 by jerrandr         ###   ########.fr       */
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
#include "Pollfd.hpp"
#include "utils.h"
#include "NotReady.hpp"

class ExecUtils
{
	private:
		std::string	getErrorUtils(std::string st,  Pollfd *polls, std::map<std::string, int> &fd_wait);	
		std::string	getStatus(std::string fl);
		std::map<std::string, std::string> Er;		
	public:
		ExecUtils &operator=(ExecUtils const & cpy) {(void)cpy;return(*this);};
		ExecUtils(ExecUtils const & cpy) {(void)cpy;};
		std::string			ToString(int nbr);
		std::string			getData(std::string filename, Pollfd *polls, std::map<std::string, int> &fd_wait);
		std::string			getError(std::string filename, Pollfd *polls, std::map<std::string, int> &fd_wait);
		std::string			getData(int fd);
		// std::vector<unsigned char>	getData(int fd);
		// std::stringstream	Uni(std::vector<unsigned char> test);
		std::string			getExt(std::string filename);
							ExecUtils();
							~ExecUtils();
};

#endif