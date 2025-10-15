/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyUpload.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 13:37:58 by jerrandr          #+#    #+#             */
/*   Updated: 2025/10/13 11:43:29 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BODYUPLOAD_HPP
#define BODYUPLOAD_HPP

#include "../utils.h"
#include "ExecUtils.hpp"
#include "../Client.hpp"

class ExecUtils;
class BodyUpload
{
	private:
		ExecUtils							*utils;
		std::vector<std::string>			filPath;
		std::vector<std::string>			Data;
		std::string							Rt;
		std::map<std::string, std::string>	vl;
					BodyUpload(BodyUpload const & cpy);
					BodyUpload();
		BodyUpload	&operator=(BodyUpload const & cpy);
		std::string	ParseHeader(std::string header);
		std::string	getSep(std::string bd);
		int 		fd_create(std::string path, Pollfd *polls, std::map<std::string, int> &fd_wait);
	public:
					BodyUpload(std::string rt);
					~BodyUpload();
		void		ParseBody(Client &cl);
		void		UploadHandler(Client &cl);
		std::string	rp_201();
		class Error409: std::exception
		{
			public:
				virtual const char *what() const throw();
		};

};

#endif