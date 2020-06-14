#ifndef VIRTUAL_MEMORY_PRINTER_H
#define VIRTUAL_MEMORY_PRINTER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace virtual_memory {
    //This class writes and output file.
    class Printer {
    public:
        //Constructor.
        Printer(const std::string &file_name);

        //Print form a vector of strings.
        void Print(const std::vector<std::string> &text);

        //Print form a string.
        void Print(const std::string &text);

        //Print the header of the functions.
        std::string HeaderToString(char type, int id_process, int memory, int edit_field);

    private:
        //Output file.
        std::ofstream output_file_;
    };
} // virtual_memory



#endif //VIRTUAL_MEMORY_PRINTER_H
