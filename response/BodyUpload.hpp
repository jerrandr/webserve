/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyUpload.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 13:37:58 by jerrandr          #+#    #+#             */
/*   Updated: 2025/09/11 14:26:31 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BODYUPLOAD_HPP
#define BODYUPLOAD_HPP

#include "../utils.h"
#include "ExecUtils.hpp"
#include "../Client.hpp"

class Client;

class BodyUpload
{
	private:
		ExecUtils							utils;
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
};

#endif