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
}
}

#endif //VIRTUAL_MEMORY_CONSTANTS_H
