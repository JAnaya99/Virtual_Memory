#ifndef VIRTUAL_MEMORY_PROCESS_H
#define VIRTUAL_MEMORY_PROCESS_H

#include "Status.h"
#include <vector>
#include <map>

namespace virtual_memory {
    // This class is a representation of a unit in the process control block.
    // It has a dictionary between the page and the index in RAM or Swapping area.
    class Process {
    public:
        //Constructor.
        Process() = default;

        //Getters.
        int GetId();

        // Return true if a page of a process is on RAM, othercase false.
        bool OnRam(int page);

        // Return true if a page of a process is on Swapping area, othercase false.
        bool OnSwapping(int page);

        // Change a page from RAM to Swapping area.
        void EraseFromRam(int page, int new_pos);

        // Change a page from Swapping area to Ram.
        void EraseFromSwapping(int page, int new_pos);

        // Return the index of a page in Swapping area.
        int GetIndexPageFromSwapping(int page);

        // Return the index of a page in RAM.
        int GetIndexPageFromRam(int page);

        // Change the index of a page in RAM.
        void SetIndexPageFromRam(int page, int new_page);

    private:
        //Dictionaries Key -> number of page, value -> index on the respective zone(RAM or Swapping).
        std::map<int, int> ram_indexes_;
        std::map<int, int> swapping_indexes_;
    };
}


#endif //VIRTUAL_MEMORY_PROCESS_H
