#ifndef VIRTUAL_MEMORY_PROCESS_H
#define VIRTUAL_MEMORY_PROCESS_H

#include "Status.h"
#include <vector>
#include <map>

namespace virtual_memory {

    class Process {
    public:
        Process() = default;

        int GetId();

        //StatusOr<int> GetPagePosition(int indx);
        bool OnRam(int page);
        bool OnSwapping(int page);

        void EraseFromRam(int page, int new_pos);
        void EraseFromSwapping(int page, int new_pos);

        int GetIndexPageFromRam(int page);
        void SetIndexPageFromRam(int page, int new_page);

    private:
        std::map<int, int> ram_indexes_;
        std::map<int, int> swapping_indexes_;
    };
}


#endif //VIRTUAL_MEMORY_PROCESS_H
