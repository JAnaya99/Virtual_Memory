#ifndef VIRTUAL_MEMORY_INSTRUCTION_H
#define VIRTUAL_MEMORY_INSTRUCTION_H

#include <string>

namespace  virtual_memory {

    enum class InstructionType {
        load = 0,
        access,
        free,
        comment,
        end,
        exit,
        undefined
    };

    class Instruction {
    public:
        Instruction();
        InstructionType type() { return type_; }
        void setType(InstructionType instruction) { type_ = instruction; }
        int process() { return process_; };
        void setProcess(int process) { process_ = process; }
        int address() { return address_; };
        void setAddress(int address) { address_ = address; }
        int numBytes() { return num_bytes_; };
        void setNumBytes(int num_bytes) { num_bytes_ = num_bytes; }
        int mode() { return mode_; };
        void setMode(int mode) { mode_ = mode; }
        std::string message() { return message_; }
        void setMessage(std::string message) { message_ = message; }

        void setUndefined(std::string message);
    private:
        InstructionType type_;
        int process_;
        int address_;
        int num_bytes_;
        int mode_;
        std::string message_;
    };

    Instruction::Instruction() {
        type_ = InstructionType::undefined;
        address_ = -1;
        mode_ = -1;
        message_ = "";
        num_bytes_ = -1;
    }

    void Instruction::setUndefined(std::string message) {
        this->setType(InstructionType::undefined);
        this->setMessage(message);
    }
}


#endif //VIRTUAL_MEMORY_INSTRUCTION_H
