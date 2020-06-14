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

    std::string Printer::HeaderToString(char type, int id_process, int memory, int edit_field){
        if(type == 'P')
            return std::string(1, type) + " ( " + std::to_string(id_process) + " , " + std::to_string(memory) + " )"; 
        else if(type == 'A')
            return std::string(1, type) + " ( " + std::to_string(id_process) + " , " + std::to_string(memory) + " , " + std::to_string(edit_field) +" )" + (edit_field == 1 ? "Write virtual memory" : "Read virtual memory");
        else if(type == 'L')
            return std::string(1, type) + " ( " + std::to_string(id_process) + " ) Free process memory";
        else if(type == 'F')
            return std::string(1, type) + "\n" + "Statistics:";
    }

} // virtual_memory