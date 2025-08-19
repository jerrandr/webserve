/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location_parsing.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: randrina <randrina@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:39:00 by randrina          #+#    #+#             */
/*   Updated: 2025/08/08 14:39:01 by randrina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

static int      insert_value(std::string key_w, std::string value, Location &lcs,
 std::vector<std::string> &found_key)
{
    if (key_w == "method" && method_check(value, found_key))
        lcs.set_meth(value);
    if (key_w == "root" && root_check(value, found_key))
        lcs.set_root(value);
    if (key_w == "autoindex")
        autoindex_check(value, lcs, found_key);
    if (key_w == "index" && index_check(value, found_key))
        lcs.set_index(value);
    if (key_w == "upload" && upload_check(value, found_key))
        lcs.set_path_upload(value);
    if (key_w == "cgi" && cgi_check(value, found_key))
        lcs.set_extension_cgi(value);
    if (key_w == "cgi_path" && cgi_path_check(value, found_key))
        lcs.set_path_cgi(value);
    if (key_w == "cgi_script" && cgi_script_check(value, found_key))
        lcs.set_script(value);
    if (key_w == "redirect" && redirect_check(value, found_key))
        lcs.set_redir(value);
    return (1);
};

static int      location_check(std::string line, Location &locs, std::vector<std::string> &found_key)
{
    std::vector<std::string>    locs_keys;
    int                         i;
    std::string                 new_line;
    std::string                 key_locs;
    std::string                 line_value;

    locs_keys.push_back("method");
    locs_keys.push_back("root");
    locs_keys.push_back("autoindex");
    locs_keys.push_back("index");
    locs_keys.push_back("upload");
    locs_keys.push_back("cgi");
    locs_keys.push_back("cgi_path");
    locs_keys.push_back("cgi_script");
    locs_keys.push_back("redirect");
    i = 0;
    while (isspace(line[i]))
        i ++;
    new_line = line.substr(i, line.size() - i);
    i = new_line.find(" ");
    key_locs = new_line.substr(0, i);
    line_value = new_line.substr(i + 1, new_line.size() - (i + 1));
    if (key_locs == "")
        return (1);
    std::vector<std::string>::iterator it = find(locs_keys.begin(), locs_keys.end(), key_locs);
    if (it != locs_keys.end())
    {
        insert_value(key_locs, line_value, locs, found_key);
        return (1);
    }
    (void) locs;
    std::cout << key_locs << ": invalid key_word !!!!!!" << std::endl;
    return (0);
};

static void     other_uri_value(std::string line, Location &locs, std::vector<std::string> &found_key)
{
    size_t     pos;
    int        len;
    std::string one_line;
    std::string other_line;

    pos = 0;
    other_line = line;
    while (pos < line.size())
    {
        len = other_line.find("\n");
        one_line = other_line.substr(0, len);
        location_check(one_line, locs, found_key);
        if (len == -1)
        break;
        pos += len + 1;
        other_line = line.substr(pos, line.size() - pos);
    };
};

int     get_principal_uri(std::string value, Config &cfg)
{
    Location                    locs;
    std::string                 uri_value;
    int                         last;
    int                         line_pos;
    std::string                 last_line;
    std::vector<std::string>    found_key;

    last = value.find(" ");
    uri_value = value.substr(0, last);
    locs.set_uri(uri_value);
    line_pos = value.find("\n") + 1;
    last_line = value.substr(line_pos, value.size() - line_pos);
    other_uri_value(last_line, locs, found_key);
    cfg.set_locs(locs);
    return (1);
};