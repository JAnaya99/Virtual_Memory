#ifndef VIRTUAL_MEMORY_READER_H
#define VIRTUAL_MEMORY_READER_H

#include <iostream>
#include <fstream>
#include "Instruction.h"

namespace virtual_memory {

    class Reader {
    public:
        Reader();
        ~Reader();
        bool addInputFile(std::string& file_name);
        StatusOr<Instruction> getNextInstruction();

    private:
        std::ifstream input_file_;
        int nextCharacterPosition(std::string& line, int pos);
        InstructionType getInstructionType(std::string& line, int& pos);
        bool isEmptyChar(char& character);
        bool isNumber(char& character);
        int getNextNumber(std::string& line, int& pos);
        void parseInstruction(Instruction& instruction, std::string& line, int& pos);
        void setAttributes(Instruction& instruction, int& first_number, int& second_number, int& third_number);
    };

    Reader::Reader() = default;

    // Destructor de la clase que se encarga de cerrar el archivo.
    Reader::~Reader() {
        input_file_.close();
    }

    StatusOr<Instruction> Reader::getNextInstruction() {
        std::string line;

        if(std::getline(input_file_, line)){

            int pos = 0;
            Instruction result;
            result.setType(getInstructionType(line, pos));
            parseInstruction(result, line, pos);

            return StatusOr<Instruction>(Status::kOk,result);
        }

        return StatusOr<Instruction>(Status::kEnd, "End of file");
    }

    bool Reader::addInputFile(std::string& file_name) {
        input_file_.open(file_name);
        return input_file_.is_open();
    }

    int Reader::nextCharacterPosition(std::string& line, int pos) {
        int internal_pos = pos;
        while(internal_pos < line.length() && isEmptyChar(line[internal_pos])) {
            internal_pos++;
        }
        return internal_pos == line.length() ? -1 : internal_pos;
    }

    InstructionType Reader::getInstructionType(std::string &line, int &pos) {
        int internal_pos = nextCharacterPosition(line, pos);

        pos = nextCharacterPosition(line, internal_pos+1);
        // Checks that a character was found and that the character in the following position is empty.
        if(internal_pos == -1 || (internal_pos + 1 < line.length() && !isEmptyChar(line[internal_pos + 1]))){
            return InstructionType::undefined;
        }
        else if(line[internal_pos] == 'P'){
            return  InstructionType::load;
        }
        else if(line[internal_pos] == 'A'){
            return  InstructionType::access;
        }
        else if(line[internal_pos] == 'L'){
            return  InstructionType::free;
        }
        else if(line[internal_pos] == 'C'){
            return  InstructionType::comment;
        }
        else if(line[internal_pos] == 'F'){
            return  InstructionType::end;
        }
        else if(line[internal_pos] == 'E'){
            return  InstructionType::exit;
        }

        return  InstructionType::undefined;
    }

    bool Reader::isEmptyChar(char &character) {
        return character == ' ' || character == '\t' || character == '\n';
    }

    bool Reader::isNumber(char &character) {
        return character >= '0' && character <= '9';
    }

    int Reader::getNextNumber(std::string &line, int &pos) {
        if(pos<0) return -1;

        int initial_pos = nextCharacterPosition(line, pos);
        if(initial_pos == -1) return pos = -1;

        int last_pos = initial_pos;
        while(last_pos < line.length() && isNumber(line[last_pos])){
            last_pos++;
        }
        if(last_pos > initial_pos){
            pos = last_pos;
            return stoi(line.substr(initial_pos,last_pos-initial_pos+1) );
        }
        return pos = -1;

    }

    void Reader::parseInstruction(Instruction &instruction, std::string &line, int &pos) {
        if(instruction.type() == InstructionType::comment){
            if(pos >= 0) instruction.setMessage(line.substr(pos));
        }
        else if (instruction.type() != InstructionType::end && instruction.type() != InstructionType::exit){
            if(pos < 0) instruction.setUndefined(const_cast<std::string &>(constants::parameters_required));
            int first_num = getNextNumber(line, pos);
            int second_num = getNextNumber(line, pos);
            int third_num = getNextNumber(line, pos);
            setAttributes(instruction, first_num, second_num, third_num);
        }
    }

    void Reader::setAttributes(Instruction &instruction, int &first_number, int &second_number, int &third_number) {
        if(instruction.type() == InstructionType::load){
            if(first_number== -1 || second_number == -1){
                instruction.setUndefined(constants::load_parameters_missing);
                return;
            }
            instruction.setNumBytes(first_number);
            instruction.setProcess(second_number);
        }
        else if(instruction.type() == InstructionType::access){
            if(first_number== -1 || second_number == -1 || third_number == -1) {
                instruction.setUndefined(constants::access_parameters_missing);
                return;
            }
            if(third_number != 0 && third_number != 1) {
                instruction.setUndefined(constants::access_mode_restriction);
                return;
            }
            instruction.setAddress(first_number);
            instruction.setProcess(second_number);
            instruction.setMode(third_number);
        }
        else if(instruction.type() == InstructionType::free){
            if(first_number == -1){
                instruction.setUndefined(constants::free_parameters_missing);
                return;
            }
            instruction.setProcess(first_number);
        }
    }

} // virtual_memory


#endif //VIRTUAL_MEMORY_PCB_H