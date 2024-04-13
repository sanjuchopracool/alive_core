#include "inae_api.h"
#include <iostream>

int main()
{
    char error[1024] = {};
    inae_init(error);
    std::cout << "inae_init error: " << error << std::endl;
    inae_de_init(error);
    std::cout << "inae_de_init error: " << error << std::endl;
    return 0;
}
