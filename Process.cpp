#include "Process.h"
#include "Status.h"
#include <vector>
#include <iostream>

namespace virtual_memory {
    bool Process::OnRam(int page) {
        return static_cast<bool>(ram_indexes_.count(page));
    }

    bool Process::OnSwapping(int page) {
        return static_cast<bool>(swapping_indexes_.count(page));
    }

    void Process::EraseFromRam(int page, int new_pos) {
        //Erase from RAM.
        ram_indexes_.erase(page);
        //Insert on swapping.
        swapping_indexes_[page] = new_pos;
    }

    void Process::EraseFromSwapping(int page, int new_pos) {
        //Erase from swapping.
        swapping_indexes_.erase(page);
        //Insert on RAM.
        ram_indexes_[page] = new_pos;
    }

    int Process::GetIndexPageFromRam(int page) {
        return ram_indexes_[page];
    }

    int Process::GetIndexPageFromSwapping(int page) {
        return swapping_indexes_[page];
    }

    void Process::SetIndexPageFromRam(int page, int new_pos) {
        ram_indexes_[page] = new_pos;
    }
}