/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Send.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 09:55:36 by jerrandr          #+#    #+#             */
/*   Updated: 2025/10/07 15:38:34 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Send.hpp"

Send::~Send() {}

Send::Send(std::string  data)
{
	_data = data;
	sent = 0;
	size = _data.size();
}

Send::Send(const Send & cpy)
{
	_data = cpy._data;
	sent = cpy.sent;
	size = cpy.size;
}

Send & Send::operator=(const Send & cpy)
{
	sent = cpy.sent;
	_data = cpy._data;
	size = cpy.size;
	return (*this);
}

std::string	& Send::getData()
{
    return (_data);
}

size_t	& Send::getSize()
{
    return (size);
}

