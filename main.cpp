/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerrandr <jerrandr@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 13:58:07 by msalohy           #+#    #+#             */
/*   Updated: 2025/10/04 12:28:15 by jerrandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

static int      open_file(std::string   input, int arg)
{
    int     fd;

    if (input != "none" && is_directory(input))
        throw std::logic_error("The input is a directory");
    if (arg)
        fd = open(input.c_str(), O_RDONLY);
    else
        fd = open("./config/default_config.cfg", O_RDONLY);
    return (fd);
};

int main(int argc, char **argv)
{
    int         fd;
    std::string input;

    if (argc > 2)
    {
        std::cout << "invalid arg" << std::endl;
        return (1); 
    }
    try
    {
        WebServer w;
        if (argc == 2)
        {
            input = argv[1];
            fd = open_file(input, 1);
        }
        else
        {
            input = "none";
            fd = open_file(input, 0);
        }
        if (fd < 0)
        {
            std::cout << "Open file failed" << std::endl;
            return (1);
        }
        w.start_webserver(fd); 
    }
    catch(SignalHandling::ExceptSTop &e)
    {
         (void)e;
    }
    catch(std::logic_error &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return (0);
}