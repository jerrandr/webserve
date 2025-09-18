/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   findLoc.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:54:45 by jerrandr          #+#    #+#             */
/*   Updated: 2025/09/18 13:48:56 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

int	Response::findLoc2(std::vector<std::string> UriLoc, std::vector<std::string> toFind)
{
	int nb;

	nb = 0;
	for (std::vector<std::string>::iterator j = UriLoc.begin(); j < UriLoc.end(); j++)
	{
		if (toFind[nb] == (*j) && nb < static_cast<int>(toFind.size()))
		{
			nb++;
			if (j == (UriLoc.end() - 1))
				break;
		}
	}
	return (nb);
}

Location Response::findLoc()
{
	std::vector<std::string>		toFind;
	std::vector<Location>::iterator fl = lc.end();
	int nb = 0;
	int max = 0;

	toFind = split(rq["uri"], "/");
	std::cout << "lc_size: " << lc.size() << "\n";
	for (std::vector<Location>::iterator i = lc.begin(); i < lc.end(); i++)
	{
		std::vector<std::string> tmp;
		tmp = split((*i).get_uri(), "/");
		std::cout << "Location: {" << (*i).get_uri() << "} URI: {" << rq["uri"] << "}" << std::endl;
		if (tmp.size() <= toFind.size())
			nb = findLoc2(tmp, toFind);
		if (max < nb)
		{
			fl = i;
			max = nb;
		}
		nb = 0;
	}
	if (fl != lc.end())
		return (*fl);
	return (*(lc.begin()));
}