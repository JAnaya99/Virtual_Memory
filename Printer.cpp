#include "Printer.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace virtual_memory{

    Printer::Printer(const std::string& file_name){
        output_file_.open(file_name.c_str());
    }

    void Printer::Print(const std::vector<std::string> &text){
        for(auto& word : text){
            output_file_ << word << std::endl;
        }
    }

    void Printer::Print(const std::string &text){
        output_file_ << text << std::endl;
    }

} // virtual_memory