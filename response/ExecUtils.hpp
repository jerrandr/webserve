/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExecUtils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 12:25:49 by jerrandr          #+#    #+#             */
/*   Updated: 2025/09/18 09:58:27 by jerrandr         ###   ########.fr       */
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
		std::string							getStatus(std::string fl);
		std::map<std::string, std::string>	Er;
		std::string							Erro;
											ExecUtils(ExecUtils const & cpy);
		ExecUtils 							&operator=(ExecUtils const & cpy);
		std::string							getFilenameError(int st, const ErrorPage & ep);
		
	public:
							ExecUtils();
							~ExecUtils();
		std::string			ToString(int nbr);
		std::string 		getData(std::string filename, Client & cl);
		std::string			getError(Client &cl, int stat);
		std::string			getData(int fd);
		bool				checkTimeOut(time_t begin, time_t end);
		std::string			getExt(std::string filename);
		std::string			CheckError(std::string	rp, Client &Cl);
		void				SendResponse(Pollfd *pl, std::string rp, int fdc);
};
							

#endif