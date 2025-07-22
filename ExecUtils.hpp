/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ExecUtils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 12:25:49 by jerrandr          #+#    #+#             */
/*   Updated: 2025/07/22 08:31:03 by jerrandr         ###   ########.fr       */
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

class ExecUtils
{
	private:
		ExecUtils(ExecUtils const & cpy);
		ExecUtils &operator=(ExecUtils const & cpy);
	public:
		std::string			ToString(int nbr);
		std::stringstream	getData(std::string filename);
		std::string			getData(int fd);
							ExecUtils();
							~ExecUtils();
};

#endif