/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msalohy <msalohy@student.42antananarivo    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:57:11 by msalohy           #+#    #+#             */
/*   Updated: 2025/07/04 11:02:25 by msalohy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

std::vector<std::string> split(std::string str, std::string sep)
{
        std::vector<std::string> nw;
        std::string tmp;
        std::size_t end;
        std::size_t start;

        tmp = "";
        start = 0;
        end = str.find(sep);
        while(end != std::string::npos)
        {
            tmp = str.substr(0,end);
            if (!tmp.empty())
                nw.push_back(tmp);
            start = end + sep.length();
            if (start >= str.length())
            {
                str = "";
                break;
            }
            str = str.substr(start, str.length());
            end = str.find(sep);
            tmp = "";
        }
        if (str != "")
            nw.push_back(str);
        return nw;
}
