/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyUpload.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 13:37:58 by jerrandr          #+#    #+#             */
/*   Updated: 2025/09/08 10:30:27 by jerrandr         ###   ########.fr       */
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
		ExecUtils					utils;
		std::vector<std::string>	Data;
		std::string					Bdy;
		std::string					Rt;
					BodyUpload(BodyUpload const & cpy);
					BodyUpload();
		BodyUpload	&operator=(BodyUpload const & cpy);
		std::string	ParseHeader(std::string header);
	public:
					BodyUpload(std::string body, std::string rt);
					~BodyUpload();
		void		ParseBody(Client &cl);
};

#endif