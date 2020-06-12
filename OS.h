#ifndef VIRTUAL_MEMORY_OS_H
#define VIRTUAL_MEMORY_OS_H

#include "Status.h"
#include "ProccessControlBlock.h"
#include "Replacement_algorithms.h"
#include "Printer.h"
#include <vector>
#include <string>
namespace virtual_memory {
    class OS {
    public:
        OS();

        //TODO: ADD EXPLANATION.
        StatusOr<std::vector<std::string>> UploadProcess(int id_process, int memory);

        //TODO: What is modify or read?
        StatusOr<std::vector<std::string>> AccessVirtualMemory(int id_process, int memory);

        //TODO: ADD EXPLANATION
        StatusOr<std::vector<std::string>> FreeProcess(int id_process, int memory);

    private:
        //Data for FIFO algorithm.
        PCB<replacement_algorithms::FIFO> pcb_FIFO_;
        Printer fifo_file_output_;

        //Data for LRU algorithm.
        PCB<replacement_algorithms::LRU> pcb_LRU_;
        Printer lru_file_output_;
    };
}


#endif //VIRTUAL_MEMORY_OS_H
