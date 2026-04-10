#include "alive_api.h"
#include <iostream>

int main()
{
#ifdef _WINDOWS
    char resource_path[] = "C:/Users/sanju/alive_resources";
#elif __APPLE__
    char resource_path[] = "/Users/sanju/alive_resources";
#else
    char resource_path[] = "/home/sanju/alive_resources";
#endif
    char error[1024] = {};
    alive_init(resource_path, error, log_level::trace);
    std::cout << "alive_init error: " << error << std::endl;
    alive_de_init(error);
    std::cout << "alive_de_init error: " << error << std::endl;
    return 0;
}
