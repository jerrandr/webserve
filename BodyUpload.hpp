/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyUpload.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 13:37:58 by jerrandr          #+#    #+#             */
/*   Updated: 2025/07/23 12:28:35 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BODYUPLOAD_HPP
#define BODYUPLOAD_HPP

#include "utils.h"
#include "ExecUtils.hpp"

class BodyUpload
{
	private:
		ExecUtils					utils;
		std::vector<std::string>	Data;
		std::string					Bdy;
					BodyUpload(BodyUpload const & cpy);
					BodyUpload();
		BodyUpload	&operator=(BodyUpload const & cpy);

	public:
					BodyUpload(std::string body);
					~BodyUpload();
		void		ParseBody();
};

#endif