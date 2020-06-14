#ifndef VIRTUAL_MEMORY_PCB_H
#define VIRTUAL_MEMORY_PCB_H


#include <vector>
#include <string>
#include <set>
#include <iostream>
#include "Process.h"
#include "Page.h"
#include "Constants.h"

namespace virtual_memory {
    // This class is a process control block. It is responsible for monitoring each process and swapping them between
    // the RAM and Swapping area.
    template<class T>
    class PCB {
    public:
        //Constructor.
        PCB();

        //Return true if the process is on PCB.
        bool OnPCB(int id_process);

        //Put a process on RAM. If it does not fit, the necessary changes will be made.
        StatusOr<std::vector<std::string>> UploadProcess(int id_process, int memory);

        //Read and write a specific virtual memory of a process.
        StatusOr<std::vector<std::string>> AccessVirtualMemory(int id_process, int memory);

        //Free all pages of a process.
        StatusOr<std::vector<std::string>> FreeProcessMemory(int id_process);

        //Return the turnaround and the total swaps in and swaps out.
        StatusOr<std::vector<std::string>> GetStatics();

    private:
        int cont_;
        int total_page_fault_;
        int timestamp_;
        int swap_in_;
        int swap_out_;

        // Dictionaries to monitoring the process.
        std::map<int, Process> index_process_;
        std::set<Page, T> running_process_;
        std::set<int> free_index_on_swapping_;
        std::set<int> free_index_on_ram_;
        std::set<int> process_ids_;
        std::map<int, int> memory_of_process_;
        std::map<std::pair<int, int>, Page> db_;
        std::map<int, int> first_timestamp_;
        std::map<int, int> final_timestamp_;

        //Private auxiliary functions.
        //Swap n pages from RAM to Swapping area.
        std::vector<std::string> RemovePages(int t_size);

        //Add n pages to RAM.
        std::vector<std::string> AddProcess(int id_process, int number_of_pages);

        //Add one page to RAM.
        std::vector<std::string> AddPageToRam(int id_process, int memory, int page_number, int move);

        //Return the index in RAM of a process.
        int GetIndexPage(int id, int memory);

        //Return a free space on swapping.
        int GetFreeSlotOnSwapping();

        //Return a free space on RAM.
        int GetFreeSlotOnRam();

        //Return the number of page of a virtual memory.
        StatusOr<int> GetNumberOfPages(int memory);

        //Join two vectors.
        void AddInformation(std::vector<std::string> &a, const std::vector<std::string> &b);

        //Transform int to string, this function only works when divide by 10.
        std::string Transform(int turnaround);
    };

    template<class T>
    StatusOr<int> PCB<T>::GetNumberOfPages(int memory) {
        //Calculate the number of pages.
        int number_of_pages = memory / constants::size_page;
        if (memory % constants::size_page != 0) {
            ++number_of_pages;
        }
        if (number_of_pages > constants::number_pages) {
            return StatusOr<int>(Status::kError, static_cast<std::string>("Memory limit exceeded"));
        }
        return StatusOr<int>(Status::kOk, number_of_pages);
    }

    template<class T>
    PCB<T>::PCB() {
        swap_in_ = 0;
        swap_out_ = 0;
        total_page_fault_ = 0;
        timestamp_ = 0;
        cont_ = 0;
        for (int i = 0; i < constants::size_swapping; i++) {
            free_index_on_swapping_.insert(i);
        }
        for (int i = 0; i < constants::number_pages; i++) {
            free_index_on_ram_.insert(i);
        }
    }

    template<class T>
    bool PCB<T>::OnPCB(int id_process) {
        return static_cast<bool>(index_process_.count(id_process));
    }

    template<class T>
    StatusOr<std::vector<std::string>> PCB<T>::UploadProcess(int id_process, int memory) {
        //Calculate the number of pages this process needs.
        StatusOr<int> s_number_of_pages = GetNumberOfPages(memory);

        //The process needs more memory than is available.
        if (s_number_of_pages.GetStatus() == Status::kError) {
            std::string log = "Status = ERROR. LOG: The process " + std::to_string(id_process) + " : " +
                              s_number_of_pages.GetMessage();
            return StatusOr<std::vector<std::string>>(Status::kError, log);
        }

        //The process is already on memory.
        if (OnPCB(id_process)) {
            std::string log =
                    "Status = ERROR. LOG: The process " + std::to_string(id_process) + " : is already on memory.";
            return StatusOr<std::vector<std::string>>(Status::kError, log);
        }

        //Save the memory of the process.
        memory_of_process_[id_process] = memory;
        process_ids_.insert(id_process);

        //Save the timestamp of the process.
        first_timestamp_[id_process] = timestamp_;

        //We need to remove some pages from ram.
        std::vector<std::string> information_process;
        if (s_number_of_pages.GetData() > static_cast<int>(free_index_on_ram_.size())) {
            information_process.push_back(
                    "There are not enough free pages in memory. Therefore the following pages will be removed");
            AddInformation(information_process,
                           RemovePages(s_number_of_pages.GetData() - static_cast<int>(free_index_on_ram_.size())));
        }

        //Add the process to the RAM.
        AddInformation(information_process, AddProcess(id_process, s_number_of_pages.GetData()));

        return StatusOr<std::vector<std::string>>(Status::kOk, information_process);
    }


    //Remove t_size indexes from memory.
    template<class T>
    std::vector<std::string> PCB<T>::RemovePages(int t_size) {
        std::vector<std::string> information_process;
        for (int i = 0; i < t_size; i++) {
            //Remove the process according to the comparator provides by the constructor.
            auto page_to_remove = *(running_process_.begin());
            running_process_.erase(page_to_remove);

            //Update the ram memory and free the positions of removed process.
            int index_page = GetIndexPage(page_to_remove.GetId(), page_to_remove.GetNumberOfPage());
            free_index_on_ram_.insert(index_page);

            //Get a empty slot in the swapping area.
            int free_slot = GetFreeSlotOnSwapping();
            index_process_[page_to_remove.GetId()].EraseFromRam(page_to_remove.GetNumberOfPage(), free_slot);

            // Add the log info to the status process.
            std::string log_process_swapping =
                    "Page " + std::to_string(page_to_remove.GetNumberOfPage()) + " of process " +
                    std::to_string(page_to_remove.GetId()) + " was changed to frame " + std::to_string(free_slot) +
                    " of the swapping area.";
            information_process.push_back(log_process_swapping);

            //A page fault ocurred.
            ++total_page_fault_;
            timestamp_ += 10;
            ++swap_out_;
        }
        return information_process;
    }

    template<class T>
    std::vector<std::string> PCB<T>::AddProcess(int id_process, int number_of_pages) {
        std::vector<std::string> information_process;
        Process process;
        for (int i = 0; i < number_of_pages; i++) {
            //Get a free slot on RAM.
            int free_slot = GetFreeSlotOnRam();
            process.SetIndexPageFromRam(i, free_slot);

            //Add one second to the timestamp.
            timestamp_ += 10;

            //Insert the process on RAM.
            running_process_.insert(Page(id_process, i, timestamp_, ++cont_, false));
            db_[std::make_pair(id_process, i)] = Page(id_process, i, timestamp_, cont_, false);

            // Add the log info to the status process.
            std::string log_process_swapping =
                    "Page " + std::to_string(i) + " of process " + std::to_string(id_process) +
                    " was placed in frame " + std::to_string(free_slot) + " of memory.";
            information_process.push_back(log_process_swapping);
        }

        index_process_[id_process] = process;
        return information_process;
    }

    template<class T>
    int PCB<T>::GetIndexPage(int id, int page) {
        return index_process_[id].GetIndexPageFromRam(page);
    }

    template<class T>
    int PCB<T>::GetFreeSlotOnSwapping() {
        int free_slot = *(free_index_on_swapping_.begin());
        free_index_on_swapping_.erase(free_slot);
        return free_slot;
    }

    template<class T>
    int PCB<T>::GetFreeSlotOnRam() {
        int free_slot = *(free_index_on_ram_.begin());
        free_index_on_ram_.erase(free_slot);
        return free_slot;
    }

    template<class T>
    StatusOr<std::vector<std::string>> PCB<T>::AccessVirtualMemory(int id_process, int memory) {
        //The process is not on memory.
        if (!OnPCB(id_process)) {
            std::string log = "Status = ERROR. LOG: The process " + std::to_string(id_process) + " : is not on memory.";
            return StatusOr<std::vector<std::string>>(Status::kError, log);
        }

        //The memory is greater than the process memory limit.
        if (memory > memory_of_process_[id_process]) {
            std::string log = "Status = ERROR. LOG: The process " + std::to_string(id_process) +
                              " : is asking for more memory than it has.";
            return StatusOr<std::vector<std::string>>(Status::kError, log);
        }

        //Get the Page of the virtual memory.
        StatusOr<int> status = GetNumberOfPages(memory + 1);
        int page_number = status.GetData() - 1;
        int move = memory - page_number * constants::size_page;


        std::vector<std::string> information_process;

        //It is not in RAM.
        if (!index_process_[id_process].OnRam(page_number)) {
            information_process.push_back("The virtual memory " + std::to_string(memory) + " is on frame " +
                                          std::to_string(
                                                  index_process_[id_process].GetIndexPageFromSwapping(page_number)) +
                                          " in swapping area.");
            //There is not more space in memory
            if (free_index_on_ram_.empty()) {
                information_process.push_back(
                        "There are not enough free pages in memory. Therefore the following pages will be removed.");
                AddInformation(information_process, RemovePages(1));
                --total_page_fault_;
            }

            // Swap-in operation.
            information_process.push_back("The page was brought into RAM (A page fault occurred).");
            timestamp_ += 10;
            ++swap_in_;
            ++total_page_fault_;
        }

        AddInformation(information_process, AddPageToRam(id_process, memory, page_number, move));

        return StatusOr<std::vector<std::string>>(Status::kOk, information_process);
    }

    template<class T>
    void PCB<T>::AddInformation(std::vector<std::string> &a, const std::vector<std::string> &b) {
        for (auto &log : b) {
            a.push_back(log);
        }
    }

    template<class T>
    std::vector<std::string> PCB<T>::AddPageToRam(int id_process, int memory, int page_number, int move) {
        std::vector<std::string> log_info;
        // Take one second to access a page.
        timestamp_ += 1;
        int secure_timestamp = timestamp_;

        //Erase from RAM and update the timestamp.
        Page page = db_[std::make_pair(id_process, page_number)];
        auto find_page = running_process_.find(page);
        int slot = 0;
        int secure_cont = page.GetArrivalNumber();
        if (find_page != running_process_.end()) {
            running_process_.erase(page);
            running_process_.insert(Page(page.GetId(), page.GetNumberOfPage(), secure_timestamp, secure_cont, false));
            slot = GetIndexPage(page.GetId(), page.GetNumberOfPage());
        } else {
            //Get a free slot on RAM.
            secure_cont = ++cont_;
            int free_slot = GetFreeSlotOnRam();
            index_process_[id_process].EraseFromSwapping(page_number, free_slot);
            running_process_.insert(Page(page.GetId(), page.GetNumberOfPage(), secure_timestamp, secure_cont, false));
            slot = free_slot;
        }

        //Update the database.
        db_[std::make_pair(id_process, page_number)] = Page(page.GetId(), page.GetNumberOfPage(), secure_timestamp,
                                                            secure_cont, false);
        log_info.push_back("Virtual memory = " + std::to_string(memory) + " -> RAM Memory = " +
                           std::to_string(move + slot * constants::size_page));
        return log_info;
    }

    template<class T>
    StatusOr<std::vector<std::string>> PCB<T>::FreeProcessMemory(int id_process) {
        //The process is not on memory.
        if (!OnPCB(id_process)) {
            std::string log = "The process " + std::to_string(id_process) + " : is not on memory.";
            return StatusOr<std::vector<std::string>>(Status::kError, log);
        }

        //Calculate the number of pages this process needs.
        int number_of_pages = GetNumberOfPages(memory_of_process_[id_process]).GetData();

        std::vector<std::string> log_info;
        log_info.push_back(std::to_string(number_of_pages) + " Pages will be released from memory");

        //Clear all pages
        for (int i = 0; i < number_of_pages; i++) {
            running_process_.erase(db_[std::make_pair(id_process, i)]);
            db_.erase(std::make_pair(id_process, i));
            if (index_process_[id_process].OnRam(i)) {
                int indx = index_process_[id_process].GetIndexPageFromRam(i);
                log_info.push_back(
                        "Page " + std::to_string(i) + " of the process was removed from frame " + std::to_string(indx) +
                        " of RAM");
                free_index_on_ram_.insert(indx);
            } else {
                int indx = index_process_[id_process].GetIndexPageFromSwapping(i);
                log_info.push_back(
                        "Page " + std::to_string(i) + " of the process was removed from frame " + std::to_string(indx) +
                        " of Swapping");
                free_index_on_swapping_.insert(indx);
            }

            //Take one second release each page.
            timestamp_ += 1;
        }

        //Delete from other data structures.
        index_process_.erase(id_process);
        memory_of_process_.erase(id_process);

        //Save the final timestamp of this process
        final_timestamp_[id_process] = timestamp_;
        return StatusOr<std::vector<std::string>>(Status::kOk, log_info);
    }

    template<class T>
    StatusOr<std::vector<std::string>> PCB<T>::GetStatics() {
        std::vector<std::string> log_info;
        //Calculate turnarounds.
        log_info.push_back("Turnarounds:");
        int turnaround_sum = 0;
        int curr_turnaround = 0;
        for (int id_process : process_ids_) {
            if (final_timestamp_.count(id_process) > 0) {
                curr_turnaround = final_timestamp_[id_process] - first_timestamp_[id_process];
            } else {
                curr_turnaround = timestamp_ - first_timestamp_[id_process];
            }

            log_info.push_back(std::to_string(id_process) + " -> \t" + Transform(curr_turnaround));
            turnaround_sum += curr_turnaround;
        }

        //Check for division by zero.
        if (!process_ids_.empty()) {
            log_info.push_back("Turnaround promedio: " + std::to_string(
                    static_cast<double>(turnaround_sum) / (static_cast<int>(process_ids_.size() * 10.0))));
        } else {
            log_info.push_back("Turnaround promedio: --");
        }

        //Create the info for the total page fault and swap-in, swap-out operations.
        log_info.push_back("Page fault: \t" + std::to_string(total_page_fault_));
        log_info.push_back("Swap in: \t\t" + std::to_string(swap_in_));
        log_info.push_back("Swap out: \t\t" + std::to_string(swap_out_));
        log_info.push_back("\n\nNew instruction block:");
        return StatusOr<std::vector<std::string>>(Status::kOk, log_info);
    }

    template<class T>
    std::string PCB<T>::Transform(int turnaround) {
        return std::to_string(turnaround / 10) + "." + std::to_string(turnaround % 10);
    }


} // virtual_memory


#endif //VIRTUAL_MEMORY_PCB_H
