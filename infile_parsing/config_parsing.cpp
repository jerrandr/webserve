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

std::string file_set_line(std::string line)
{
    std::string new_line;
    std::string key_w;
    std::string one_line;
    int         last;
    int         i;

    i = 0;
    last = line.find("\n");
    one_line = line.substr(0, last);
    while (isspace(one_line[i]))
        i ++;
    new_line = line.substr(i, line.size() - i);  
    last = new_line.find(" ");
    key_w = one_line.substr(i, last);
    if (key_w == "location")
    {
        last = new_line.find("}");
        one_line = new_line.substr(0, last);
    }
    return (one_line); 
};
static int      keys_error_handling(std::string line)
{
    std::vector<std::string> all_keys;
    all_keys.push_back("server");
    all_keys.push_back("host");
    all_keys.push_back("listen");
    all_keys.push_back("error_page");
    all_keys.push_back("client_max_body_size");
    all_keys.push_back("location");
    all_keys.push_back("method");
    all_keys.push_back("root");
    all_keys.push_back("autoindex");
    all_keys.push_back("index");
    all_keys.push_back("upload");
    all_keys.push_back("cgi");
    all_keys.push_back("{");       
    all_keys.push_back("}");       
    all_keys.push_back("#");
    std::vector<std::string>::iterator it = find(all_keys.begin(), all_keys.end(), line);
    if (it != all_keys.end())
        return (1);
    return (0);
};

static int     other_checking(std::string key_w, std::string value, Config &cfg)
{
    if (key_w == "client_max_body_size")
        body_size_checking(value, cfg);
    if (key_w == "location")
        get_principal_uri(value, cfg);
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
        line = file_set_line(new_input);
        len = line.size();
        if (!line_parsing(line, cfg, err_page, err_vector))
        {
            std::cout << "Error of configuration file content !!!" << std::endl;
            break;
        }
        if (len == -1)
            break ;
        pos += len + 1;
        if (pos < input.size())
            new_input = input.substr(pos,input.size() - pos);
        else
            new_input = input.substr(pos -1 , input.size() - (pos - 1));
    };
    error_page_occurences(err_vector);
    cfg.set_errors(err_page);
};

void    config_parsing(int fd, Config &cfg)
{
    std::string         all_string;
    size_t              length;
    char                buffer[100];
    ErrorPage           err_p;

    (void) cfg;
    length = 100;
    all_string = "";
    std::memset(buffer, 0, 100);
    while (read(fd, buffer, length - 1) > 0)
    {
        all_string +=  buffer;
        std::memset(buffer, 0, 100);
    };
    std::cout << "-----------------------string-----------------" << std::endl;
    std::cout << all_string << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    get_all_line(all_string, cfg);
    std::cout << "---------all_content---------" << std::endl;
    std::cout << cfg.get_host() << std::endl;
    std::cout << cfg.get_port() << std::endl;
    err_p = cfg.get_errors();
    std::cout << err_p.get_path_400() << std::endl;
    std::cout << err_p.get_path_404() << std::endl;
    std::cout << err_p.get_path_405() << std::endl;
    std::cout << cfg.get_max_allowed_size() << std::endl;

    std::cout << "---------location------------" << std::endl;
    std::cout << "-----------------------------" << std::endl;
};
