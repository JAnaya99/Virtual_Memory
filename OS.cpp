#include "OS.h"
#include "Status.h"
#include "Constants.h"

namespace virtual_memory {
    OS::OS() : lru_file_output_(constants::LRU_file_path), fifo_file_output_(constants::FIFO_file_path) {}

    StatusOr<std::vector<std::string>> OS::UploadProcess(int id_process, int memory) {
        StatusOr<std::vector<std::string>> status = pcb_LRU_.UploadProcess(id_process, memory);
        if (status.GetStatus() == Status::kError){
            lru_file_output_.Print(status.GetMessage());
        }else{
            lru_file_output_.Print(status.GetData());
        }
        return pcb_LRU_.UploadProcess(id_process, memory);
    }

    StatusOr<std::vector<std::string>> OS::AccessVirtualMemory(int id_process, int memory) {
        return pcb_LRU_.AccessVirtualMemory(id_process, memory);
    }
} // virtual_memory
