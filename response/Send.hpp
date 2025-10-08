/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Send.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 13:41:25 by jerrandr          #+#    #+#             */
/*   Updated: 2025/10/07 15:38:08 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEND_HPP
#define SEND_HPP
#include "Response.hpp"

class Send
{
	private:
		std::string	_data;
		size_t		size;
	public:
		size_t		sent;
		~Send();
		Send(const Send & cpy);
		Send & operator=(const Send & cpy);
		size_t	& getSize();
		std::string	&getData();
		Send(std::string data);
};

#endif