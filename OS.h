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
        void UploadProcess(int id_process, int memory);

        //TODO: What is modify or read?
        void AccessVirtualMemory(int id_process, int memory, int edit_field);

        //TODO: ADD EXPLANATION
        void FreeProcess(int id_process, int memory);

        
    private:
        //Data for FIFO algorithm.
        PCB<replacement_algorithms::FIFO> pcb_FIFO_;
        Printer fifo_file_output_;

        //Data for LRU algorithm.
        PCB<replacement_algorithms::LRU> pcb_LRU_;
        Printer lru_file_output_;

        //TODO: ADD  EXPLANATION
        void PrintHeaders(char type, int id_process, int memory_or_page = -1, int edit_field = -1);
        void PrintResults(Printer& a, StatusOr<std::vector<std::string>>& status);
    };
}


#endif //VIRTUAL_MEMORY_OS_H
