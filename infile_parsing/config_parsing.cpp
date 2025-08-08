/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parsing.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: randrina <randrina@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 09:11:58 by randrina          #+#    #+#             */
/*   Updated: 2025/08/08 12:45:40 by randrina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
#include "../utils.h"
static int      body_size_checking(std::string value, Config &cfg)
{
    std::cout << "body_size-> " << value << std::endl;
    cfg.set_max_allowed_size(value);
    return (1);
};

static int      keys_error_handling(std::string line)
{
    std::vector<std::string> all_keys;
    std::string key_array[] = {"server", "host", "listen", "error_page",
         "client_max_body_size", "location", "method", "root", "autoindex",
          "index", "upload", "cgi", "{", "}", "#"};
    for (size_t i = 0; i < key_array->size(); i ++)
        all_keys.push_back(key_array[i]);
    std::vector<std::string>::iterator it = find(all_keys.begin(), all_keys.end(), line);
    if (it != all_keys.end())
        return (1);
    return (0);
};

static int     other_checking(std::string key_w, std::string value, Config &cfg)
{
    if (key_w == "client_max_body_size")
        body_size_checking(value, cfg);
    return (1);
};

static int     line_parsing(std::string line, Config &cfg, ErrorPage &err_page,
     std::vector<std::string> &err_vector)
{
    int             last;
    size_t          i;
    std::string     key_w;
    std::string     new_line;
    std::string     new_values;
    
    i = 0;
    while (isspace(line[i]))
        i ++;
    new_line = line.substr(i, line.size() - i);
    last = new_line.find(" ");
    key_w = new_line.substr(0, last);
    if (!keys_error_handling(key_w))
        return (0);
    new_values = new_line.substr(last + 1, new_line.size() - last);
    if (key_w == "host")
       address_check(new_values, cfg);
    if (key_w == "listen")
        port_check(new_values, cfg);
    if (key_w == "error_page")
        error_page_set(new_values, err_page, err_vector);
    other_checking(key_w, new_values, cfg);
    return (1);    
};

static void     get_all_line(std::string input, Config &cfg)
{
    int                                 len;
    size_t                              pos;
    std::string                         line;
    std::string                         new_input;
    std::vector<std::string>            key_vect;
    ErrorPage                           err_page;
    std::vector<std::string>            err_vector;

    pos = 0;
    new_input = input;
    while (pos < input.size())
    {
        len = new_input.find("\n");
        line = input.substr(pos,len);
        if (!line_parsing(line, cfg, err_page, err_vector))
        {
            std::cout << "Error content" << std::endl;
            break;
        }
        if (len == -1)
            break ;
        pos += len + 1;
        new_input = input.substr(pos,input.size() - pos);
    };
    error_page_occurences(err_vector);
    cfg.set_errors(err_page);
};

void    config_parsing(int fd, Config &cfg)
{
    std::string         string;
    size_t              length;
    char                buffer[100];
    ErrorPage           err_p;

    (void) cfg;
    length = 100;
    string = "";
    std::memset(buffer, 0, 100);
    while (read(fd, buffer, length - 1) > 0)
    {
        string +=  buffer;
        std::memset(buffer, 0, 100);
    };
    get_all_line(string, cfg);
    std::cout << "---------all_content---------" << std::endl;
    std::cout << cfg.get_host() << std::endl;
    std::cout << cfg.get_port() << std::endl;
    err_p = cfg.get_errors();
    std::cout << err_p.get_path_400() << std::endl;
    std::cout << err_p.get_path_404() << std::endl;
    std::cout << err_p.get_path_405() << std::endl;
    std::cout << cfg.get_max_allowed_size() << std::endl;
    std::cout << "-----------------------------" << std::endl;
};
