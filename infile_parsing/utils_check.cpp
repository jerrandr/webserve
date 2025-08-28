#include "../Server.hpp"

int     multiple_key_check(std::vector<std::string> &found_key)
{
    if (found_key.size() == 0)
        return (1);
    for (size_t i = 0; i < found_key.size() - 1; i ++)
    {
        for (size_t j = i + 1; j < found_key.size(); j ++)
        {
            if (found_key[i] == found_key[j])
                return (0);
        };
    };
    return (1);
};

int check_all_error_path(Config &cfg)
{
    std::vector<std::string>    all_path;
    ErrorPage                   err_page;

    err_page = cfg.get_errors();
    all_path.push_back(err_page.get_path_400());
    all_path.push_back(err_page.get_path_403());
    all_path.push_back(err_page.get_path_404());
    all_path.push_back(err_page.get_path_405());
    all_path.push_back(err_page.get_path_406());
    all_path.push_back(err_page.get_path_408());
    all_path.push_back(err_page.get_path_410());
    all_path.push_back(err_page.get_path_411());
    all_path.push_back(err_page.get_path_413());
    all_path.push_back(err_page.get_path_414());
    all_path.push_back(err_page.get_path_415());
    all_path.push_back(err_page.get_path_417());
    all_path.push_back(err_page.get_path_500());
    all_path.push_back(err_page.get_path_501());
    all_path.push_back(err_page.get_path_502());
    all_path.push_back(err_page.get_path_503());
    all_path.push_back(err_page.get_path_504());
    all_path.push_back(err_page.get_path_505());
    for (size_t i = 0; i < all_path.size(); i ++)
    {
        if (access(all_path[i].c_str(), F_OK || R_OK) == -1)
            return (0);
    };
    return (1); 
};