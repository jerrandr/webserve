#include <iostream>
#include "directory_listing.h"

int main(int argc, char **argv)
{
    (void)argc;
    std::string name(argv[1]);

    directory_listing(name);
}
