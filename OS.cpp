#include "OS.h"
#include "Status.h"
#include "Constants.h"




namespace virtual_memory {
    OS::OS() : lru_file_output_(constants::LRU_file_path), fifo_file_output_(constants::FIFO_file_path) {}

    void OS::UploadProcess(int id_process, int memory) {
        //Print the comand.
        PrintHeaders('P', id_process, memory);

        //Upload process with a LRU and FIFO replacement algorithm.
        StatusOr<std::vector<std::string>> lru_status = pcb_LRU_.UploadProcess(id_process, memory);
        PrintResults(lru_file_output_, lru_status);
        StatusOr<std::vector<std::string>> fifo_status = pcb_FIFO_.UploadProcess(id_process, memory);
        PrintResults(fifo_file_output_, fifo_status);
    }

    void OS::AccessVirtualMemory(int id_process, int memory, int edit_field) {
        //Print the comand.
        PrintHeaders('A', id_process, memory, edit_field);

        //Access to the process (id_process) with virtual memory (memory).
        StatusOr<std::vector<std::string>> lru_status = pcb_LRU_.AccessVirtualMemory(id_process, memory);
        PrintResults(lru_file_output_, lru_status);
        StatusOr<std::vector<std::string>> fifo_status = pcb_FIFO_.AccessVirtualMemory(id_process, memory);
        PrintResults(fifo_file_output_, fifo_status);
    }

    void OS::PrintHeaders(char type, int id_process, int memory_or_page, int edit_field){
        //Print the comand in both files.
        lru_file_output_.Print(lru_file_output_.HeaderToString(type, id_process, memory_or_page, edit_field));
        fifo_file_output_.Print(fifo_file_output_.HeaderToString(type, id_process, memory_or_page, edit_field));
    }

    void OS::PrintResults(Printer& file_output, StatusOr<std::vector<std::string>>& status){
        if (status.GetStatus() == Status::kError){
            file_output.Print(status.GetMessage());
        }else{
            file_output.Print(status.GetData());
        }
    }
} // virtual_memory
