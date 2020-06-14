#ifndef VIRTUAL_MEMORY_PCB_H
#define VIRTUAL_MEMORY_PCB_H


#include <vector>
#include <string>
#include <set>
#include <iostream>
#include "Process.h"
#include "Page.h"
#include "Constants.h"

//TODO: Add comments.
namespace virtual_memory {
    template<class T>
    class PCB {
    public:
        PCB();

        bool OnPCB(int id_process);

        StatusOr<std::vector<std::string>> UploadProcess(int id_process, int memory);

        StatusOr<std::vector<std::string>> AccessVirtualMemory(int id_process, int memory);

    private:
        int cont_;
        int total_page_fault_;
        double timestamp_;

        std::map<int, Process> index_process_;
        std::set<Page, T> running_process_;
        std::set<int> free_index_on_swapping_;
        std::set<int> free_index_on_ram_;
        std::map<int, int> memory_of_process_;
        std::map<std::pair<int, int>, Page> db_;

        //Remove t_size indexes from memory.
        std::vector<std::string> RemovePages(int t_size);

        std::vector<std::string> AddProcess(int id_process, int number_of_pages);
        std::vector<std::string> AddPageToRam(int id_process, int memory, int page_number, int move);

        int GetIndexPage(int id, int memory);

        int GetFreeSlotOnSwapping();

        int GetFreeSlotOnRam();

        StatusOr<int> GetNumberOfPages(int memory);

        void AddInformation(std::vector<std::string>& a, const std::vector<std::string>& b); 

    };

    template<class T>
    StatusOr<int> PCB<T>::GetNumberOfPages(int memory) {
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
        total_page_fault_ = 0;
        timestamp_ = 0.0;
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
            std::string log = "The process " + std::to_string(id_process) + " : is already on memory.";
            return StatusOr<std::vector<std::string>>(Status::kError, log);
        }

        //Save the memory of the process.
        memory_of_process_[id_process] = memory;

        //We need to remove some pages from ram.
        std::vector<std::string> information_process;
        if (s_number_of_pages.GetData() > static_cast<int>(free_index_on_ram_.size())) {
            information_process.push_back("There are not enough free pages in memory. Therefore the following pages will be removed");
            AddInformation(information_process, RemovePages(s_number_of_pages.GetData() - static_cast<int>(free_index_on_ram_.size())));
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
            index_process_[page_to_remove.GetId()].EraseFromRam(index_page, free_slot);

            // Add the log info to the status process.
            std::string log_process_swapping = "Page " + std::to_string(page_to_remove.GetNumberOfPage()) + " of process " + std::to_string(page_to_remove.GetId()) + " was changed to frame " + std::to_string(index_page) + " of the swapping area.";
            information_process.push_back(log_process_swapping);

            //A page fault ocurred.
            ++total_page_fault_;
            timestamp_ += 0.1;
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
            timestamp_ += 1.0;

            //Insert the process on RAM.
            running_process_.insert(Page(id_process, i, timestamp_, ++cont_, false));
            db_[std::make_pair(id_process, i)] = Page(id_process, i, timestamp_, ++cont_, false);

            // Add the log info to the status process.
            std::string log_process_swapping = "Page " + std::to_string(i) + " of process " + std::to_string(id_process) + " was placed in frame " + std::to_string(free_slot) + " of memory.";
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
            std::string log = "The process " + std::to_string(id_process) + " : is not on memory. \n";
            return StatusOr<std::vector<std::string>>(Status::kError, log);
        }

        //The memory is greater than the process memory limit.
        if (memory > memory_of_process_[id_process]){
            std::string log = "The process " + std::to_string(id_process) + " : is asking for more memory than it has. \n";
            return StatusOr<std::vector<std::string>>(Status::kError, log);
        }

        //Get the Page of the virtual memory.
        StatusOr<int> status = GetNumberOfPages(memory + 1);
        int page_number = status.GetData() - 1;
        int move = memory - page_number * constants::size_page;


        std::vector<std::string> information_process;

        //It is not in RAM.
        if (!index_process_[id_process].OnRam(page_number)) {
            information_process.push_back("The virtual memory " + std::to_string(memory) + " is on frame " + std::to_string(index_process_[id_process].GetIndexPageFromSwapping(page_number)) + " in swapping area.");
            //There is not more space in memory
            if(free_index_on_ram_.empty()){
                information_process.push_back("There are not enough free pages in memory. Therefore the following pages will be removed.");
                AddInformation(information_process, RemovePages(1));
            }
            information_process.push_back("The page was brought into RAM (A page fault occurred).");
            ++total_page_fault_;
        }

        AddInformation(information_process, AddPageToRam(id_process, memory, page_number, move));
        
        return StatusOr<std::vector<std::string>>(Status::kOk, information_process);
    }

    template<class T>
    void PCB<T>::AddInformation(std::vector<std::string>& a, const std::vector<std::string>& b){
        for(auto& log : b){
            a.push_back(log);
        }
    }

    template<class T>
    std::vector<std::string> PCB<T>::AddPageToRam(int id_process, int memory, int page_number, int move){
        std::vector<std::string> log_info;
        timestamp_ += 0.1;
        
        //Erase from RAM and update the timestamp.
        Page page = db_[std::make_pair(id_process, page_number)];
        auto find_page = running_process_.find(page);
        int slot = 0;
        if(find_page != running_process_.end()){
            running_process_.erase(page);
            running_process_.insert(Page(page.GetId(), page.GetNumberOfPage(), timestamp_, cont_, false));
            slot = GetIndexPage(page.GetId(), page.GetNumberOfPage());
        }else{
            //Get a free slot on RAM.
            int free_slot = GetFreeSlotOnRam();
            index_process_[id_process].SetIndexPageFromRam(page_number, free_slot);
            running_process_.insert(Page(page.GetId(), page.GetNumberOfPage(), timestamp_, ++cont_, false));
            slot = free_slot;
        }

        //Update the database.
        db_[std::make_pair(id_process, page_number)] = Page(page.GetId(), page.GetNumberOfPage(), timestamp_, cont_, false);
        
        log_info.push_back("Virtual memory = " + std::to_string(memory) + " -> RAM Memory = " + std::to_string(move + slot * constants::size_page));
        return log_info;
    }


} // virtual_memory


#endif //VIRTUAL_MEMORY_PCB_H
