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

static void     root_check_path(Location &locs)
{
    std::string     str;
    std::string     uri_str;
    std::string     upl_str;

    str = locs.get_root();
    uri_str = locs.get_uri();
    upl_str = locs.get_path_upload();
    if (str.empty() || uri_str.empty())
        return ;
    if (str[str.size() - 1] != '/')
    {
        str += "/";
        locs.set_root(str);
    }
    if (uri_str != "/" && uri_str[uri_str.size() - 1] != '/')
    {
        uri_str += "/";
        locs.set_uri(uri_str);
    };
    if (!upl_str.empty() && upl_str != "no" && upl_str != "NO" &&
         upl_str[upl_str.size() - 1] != '/')
    {
        upl_str += "/";
        locs.set_path_upload(upl_str);
    }
};
static int      insert_value(std::string key_w, std::string value, Location &lcs,
 std::vector<std::string> &found_key)
{
    if (key_w == "method")
    {
        if (method_check(value, found_key))
            lcs.set_meth(value);
        else
            return (0);
    };
    if (key_w == "root")
    {
        if (root_check(value, found_key))    
            lcs.set_root(value);
        else 
            return (0);
    };
    if (key_w == "autoindex")
    {
        if (!autoindex_check(value, lcs, found_key))
            return (0);
    };
    if (key_w == "index")
    {
        if (index_check(value, found_key))
            lcs.set_index(value);
        else
            return (0);
    };
    if (key_w == "upload")
    {
        if (upload_check(value, found_key))
            lcs.set_path_upload(value);
        else
            return (0);
    };  
    if (key_w == "cgi")
    {
        if (cgi_check(value, found_key))
            lcs.set_extension_cgi(value);
        else
            return (0);
    }
    if (key_w == "cgi_path")
    {
        if (cgi_path_check(value, found_key))
            lcs.set_path_cgi(value);
        else
            return (0);
    }
    if (key_w == "redirect")
    {
        if (redirect_check(value, found_key))
            lcs.set_redir(value);
        else
            return (0);
    }
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
        if (insert_value(key_locs, line_value, locs, found_key))
            return (1);
        else
            return (0);
    };
    return (0);
};

static int     other_uri_value(std::string line, Location &locs, std::vector<std::string> &found_key)
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
        if (!location_check(one_line, locs, found_key))
            return (0);
        if (len == -1)
        break;
        pos += len + 1;
        other_line = line.substr(pos, line.size() - pos);
    };
    return (1);
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
    if (!other_uri_value(last_line, locs, found_key))
        return (0);
    if (!multiple_key_check(found_key))
        return (0);
    root_check_path(locs);
    cfg.set_locs(locs);
    return (1);
};