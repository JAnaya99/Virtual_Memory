//nombres

#include <iostream>
#include <vector>
#include <string>
#include "OS.h"
#include "Reader.h"

using namespace std;

int main(){
    virtual_memory::OS system;
    virtual_memory::Reader reader;

    std::string file_name;
    std::cout << "Ingresa el PATH del archivo de instrucciones:  " << std::endl;
    std::cin >> file_name;

    if(reader.addInputFile(file_name)){
        virtual_memory::StatusOr<virtual_memory::Instruction> current_instruction = reader.getNextInstruction();
        while(current_instruction.GetStatus() == virtual_memory::Status::kOk){
            virtual_memory::InstructionType current_type = current_instruction.GetData().type();
            virtual_memory::Instruction instruction = current_instruction.GetData();
            if(current_type == virtual_memory::InstructionType::load){
                // Load a process
                cout << "Instruccion de carga" << endl;
                cout << instruction.numBytes() << ", process: " << instruction.process() << endl;
            }
            else if(current_type == virtual_memory::InstructionType::access){
                // Access to address
                cout << "Instruccion de acceso" << endl;
                cout << instruction.address() << ", process: " << instruction.process() << ", mode= " << instruction.mode() << endl;
            }
            else if(current_type == virtual_memory::InstructionType::free){
                // Free pages
                cout << "Instruccion de liberar" << endl;
                cout << "process: " << instruction.process() << endl;
            }
            else if(current_type == virtual_memory::InstructionType::comment){
                // Add comment
                cout << "Instruccion de comentario" << endl;
                cout << "Comment: " << instruction.message() << endl;
            }
            else if(current_type == virtual_memory::InstructionType::end){
                // End of the request set
                cout << "Instruccion de termino de conjunto" << endl;
            }
            else if(current_type == virtual_memory::InstructionType::exit){
                // Exit program
                cout << "Instruccion de salida" << endl;
            }
            else {
                // Output ignoring type
                cout << "Instruccion incorrecta" << endl;
                cout << "Message: " << instruction.message() << endl;
            }
            current_instruction = reader.getNextInstruction();
        }
    }
    else{
        cout << "ERROR: El archivo no existe. Revisa el nombre ingresado" << endl;
    }

    return 0;
}
