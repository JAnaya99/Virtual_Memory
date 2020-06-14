#ifndef VIRTUAL_MEMORY_CONSTANTS_H
#define VIRTUAL_MEMORY_CONSTANTS_H

#include <string>

namespace virtual_memory{
namespace constants{
    constexpr int size_memory = 2048;
    constexpr int size_page = 16;
    constexpr int number_pages = size_memory / size_page;
    constexpr int size_swapping = 4092;
    const std::string LRU_file_path = "LRU_OUTPUT_FILE.txt";
    const std::string FIFO_file_path = "FIFO_OUTPUT_FILE.txt";
    const std::string parameters_required = "Parameters are missing";
    const std::string load_parameters_missing = "Load instructions require 2 parameters";
    const std::string access_parameters_missing = "Access/modify instructions require 3 parameters";
    const std::string free_parameters_missing = "Free memory instructions require 1 parameter";
    const std::string access_mode_restriction = "Last parameter in access instructions can only be 0 or 1";
}
}

#endif //VIRTUAL_MEMORY_CONSTANTS_H
