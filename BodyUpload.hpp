/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BodyUpload.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 13:37:58 by jerrandr          #+#    #+#             */
/*   Updated: 2025/07/22 13:50:38 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BODYUPLOAD_HPP
#define BODYUPLOAD_HPP

#include <iostream>
#include <cstring>

class BodyUpload
{
	private:
					BodyUpload(BodyUpload const & cpy);
		BodyUpload	&operator=(BodyUpload const & cpy);
					BodyUpload();
		std::string	Bdy;
	public:
		BodyUpload(std::string body);
		~BodyUpload();
};

#endif