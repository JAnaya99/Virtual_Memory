#ifndef VIRTUAL_MEMORY_OS_H
#define VIRTUAL_MEMORY_OS_H

#include "Status.h"
#include "ProccessControlBlock.h"
#include "Replacement_algorithms.h"
#include "Printer.h"
#include <vector>
#include <string>

namespace virtual_memory {
    //This class is the operative system. It call all the functions.
    class OS {
    public:
        OS();

        //Put a process on RAM. If it does not fit, the necessary changes will be made.
        void UploadProcess(int id_process, int memory);

        //Read and write a specific virtual memory of a process.
        void AccessVirtualMemory(int id_process, int memory, int edit_field);

        //Free all pages of a process.
        void FreeProcess(int id_process);

        //Return the turnaround and the total swaps in and swaps out.
        void RestartMemory();

    private:
        //Data for FIFO algorithm.
        PCB<replacement_algorithms::FIFO> pcb_FIFO_;
        Printer fifo_file_output_;

        //Data for LRU algorithm.
        PCB<replacement_algorithms::LRU> pcb_LRU_;
        Printer lru_file_output_;

        //Print the header of the command.
        void PrintHeaders(char type, int id_process = -1, int memory_or_page = -1, int edit_field = -1);

        //Print both results FIFO and LRU.
        void PrintResults(Printer &a, StatusOr<std::vector<std::string>> &status);
    };
}


#endif //VIRTUAL_MEMORY_OS_H
