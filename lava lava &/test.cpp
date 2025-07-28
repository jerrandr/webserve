#include <iostream>
#include <sstream>

std::string decode_str(std::string d)
{
    std::string tmp;
    std::stringstream ss;

    tmp = "";

    for(std::size_t i = 0; i < d.size();i++)
    {
         if (d[i] == '%' && i+2 < d.size())
         {
            int x;
            std::string h;

            h = "";
            h += d[i+1]+d[i+2];
            std::cout <<"{"<<h <<"}"<< std::endl;
            x = strtol(h.c_str(),NULL,16);
            std::cout << x << std::endl;
            tmp += static_cast<char>(x);
            i+=2;

         }
         else if (d[i] == '+')
            tmp += " ";
         else
            tmp += d[i];
    }
    return tmp;
}
int main()
{
    std::string tmp;

    tmp  = decode_str("http://localhost:8080/lava%20lava%20&/");
    std::cout << tmp << std::endl;
    return (0);
}