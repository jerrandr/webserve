#include "../Server.hpp"

int     multiple_key_check(std::vector<std::string> &found_key)
{
    if (found_key.size() == 0)
        return (0);
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