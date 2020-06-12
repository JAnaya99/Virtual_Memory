#ifndef VIRTUAL_MEMORY_PRINTER_H
#define VIRTUAL_MEMORY_PRINTER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace virtual_memory{
    class Printer {
    public:
        Printer(const std::string& file_name);

        void Print(const std::vector<std::string> &text);
        void Print(const std::string &text);

    private:
        std::ofstream output_file_;
    };
} // virtual_memory



#endif //VIRTUAL_MEMORY_PRINTER_H
