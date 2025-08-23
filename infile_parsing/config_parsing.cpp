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

static int      check_one_bloc(std::string bloc)
{
    size_t i = 0;
    std::string new_bloc;

    if (bloc[0] == '\n' && bloc[1] != '{')
    {
        std::cout << "Some error in the configuration." << std::endl;
        return (0);
    };
    while (isspace(bloc[i]) || bloc[i] == '\n')
        i ++;
    new_bloc = bloc.substr(i, bloc.size() - i);
    if (new_bloc.empty() || new_bloc[0] != '{')
    {
        std::cout << "Server bloc syntax error" << std::endl;
        return (0);
    }
    i = 0;
    while (isspace(bloc[i]) || bloc[i] == '{' || bloc[i] == '}')
        i ++;
    new_bloc = bloc.substr(i, bloc.size() - i);
    if (new_bloc.empty())
    {
        std::cout << "Empty content in one bloc." << std::endl;
        return (0);
    }
    return (1);
};

static int      body_size_checking(std::string value, Config &cfg, std::vector<std::string> &key_vect)
{
    std::stringstream   tmp;
    double              nbr_value;
    std::string         last;

    key_vect.push_back("max_body_size");
    tmp << value;
    tmp >> nbr_value;
    tmp >> last;
    if (!last.empty() && last != "G" && last != "M" && last != "K" && 
        last != "g" && last != "m" && last != "k")
        std::cout << "Invalid body_size unity" << std::endl;
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
    all_keys.push_back("host");
    all_keys.push_back("listen");
    all_keys.push_back("error_page");
    all_keys.push_back("client_max_body_size");
    all_keys.push_back("location");
    all_keys.push_back("{");       
    all_keys.push_back("}");
    if (line.empty())
        return (1);       
    std::vector<std::string>::iterator it = find(all_keys.begin(), all_keys.end(), line);
    if (it != all_keys.end())
        return (1);
    return (0);
};

static int     other_checking(std::string key_w, std::string value, Config &cfg,
    std::vector<std::string> &key_vect)
{
    if (key_w == "client_max_body_size")
        body_size_checking(value, cfg, key_vect);
    if (key_w == "location")
        get_principal_uri(value, cfg);
    return (1);
};

static int     line_parsing(std::string line, Config &cfg, ErrorPage &err_page,
     std::vector<std::string> &err_vector, std::vector<std::string> &key_vect)
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
       address_check(new_values, cfg, key_vect);
    if (key_w == "listen")
        port_check(new_values, cfg, key_vect);
    if (key_w == "error_page")
    {

        if (!error_page_set(new_values, err_page, err_vector))
            return (0);
    };  
    other_checking(key_w, new_values, cfg, key_vect);
    return (1);    
};

static Config     get_all_line(std::string input, int fd)
{
    int                                 len;
    size_t                              pos;
    std::string                         line;
    std::string                         new_input;
    std::vector<std::string>            key_vect;
    ErrorPage                           err_page;
    std::vector<std::string>            err_vector;
    std::vector<std::string>            found_key;
    Config                              config;

    pos = 0;
    new_input = input;
    while (pos < input.size())
    {
        line = file_set_line(new_input);
        len = line.size();
        if (!line_parsing(line, config, err_page, err_vector, found_key))
        {
            close (fd);
            throw std::logic_error("Error of configuration file content !!!");
        };
        if (len == -1)
            break ;
        pos += len + 1;
        if (pos < input.size())
            new_input = input.substr(pos,input.size() - pos);
        else
            new_input = input.substr(pos -1 , input.size() - (pos - 1));
    };

    if (!multiple_key_check(found_key) ||  !error_page_occurences(err_vector))
    {
        close (fd);
        throw std::logic_error("Multiple definition of keyword!!!");
    };
    config.set_errors(err_page);
    return(config);
};

void    config_parsing(int fd, std::vector<Config> &cfg)
{
    std::string                 all_string;
    size_t                      length;
    char                        buffer[100];
    ErrorPage                   err_p;
    std::vector<std::string>    string_array;
    Config                      config;

    length = 100;
    all_string = "";
    std::memset(buffer, 0, 100);
    while (read(fd, buffer, length - 1) > 0)
    {
        all_string +=  buffer;
        std::memset(buffer, 0, 100);
    };
    string_array = split(all_string, "server");
    std::vector<std::string>::iterator it = string_array.begin();
    while (it != string_array.end())
    {
        if (!check_one_bloc(*it))
        {
            close(fd);
            throw std::logic_error("Error in the server bloc !!!");
        }
        config = get_all_line(*it, fd);
        if (!check_port_and_host(config))
        {
            close(fd);
            throw std::logic_error("Port and host error !!!");
        }
        cfg.push_back(config);
        it ++;
    }
    close(fd);
};