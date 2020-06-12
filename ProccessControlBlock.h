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


        std::map<int, Process> index_process_;
        std::set<Page, T> running_process_;
        std::set<int> free_index_on_swapping_;
        std::set<int> free_index_on_ram_;

        //Remove t_size indexes from memory.
        void RemovePages(int t_size);

        void AddProcess(int id_process, int number_of_pages);

        int GetIndexPage(int id, int memory);

        int GetFreeSlotOnSwapping();

        int GetFreeSlotOnRam();

        StatusOr<int> GetNumberOfPages(int memory);


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
            std::string log = "The process " + std::to_string(id_process) + " : is already on memory. \n";
            return StatusOr<std::vector<std::string>>(Status::kError, log);
        }

        //We need to remove some pages from ram.
        if (s_number_of_pages.GetData() > static_cast<int>(free_index_on_ram_.size())) {
            RemovePages(s_number_of_pages.GetData() - static_cast<int>(free_index_on_ram_.size()));
        }

        //TODO: ADD THE FUNCTION ADD;
        AddProcess(id_process, s_number_of_pages.GetData());

        std::vector<std::string> information_process{"Good"};
        return StatusOr<std::vector<std::string>>(Status::kOk, information_process);
    }


    //Remove t_size indexes from memory.
    template<class T>
    void PCB<T>::RemovePages(int t_size) {
        for (int i = 0; i < t_size; i++) {
            //Remove the process according to the comparator provides by the constructor.
            auto page_to_remove = *(running_process_.begin());
            running_process_.erase(page_to_remove);

            //Update the ram memory and free the positions of removed process.
            //std::cout << "Remove: " << page_to_remove.GetId() << " " << page_to_remove.GetNumberOfPage() << " ";
            int index_page = GetIndexPage(page_to_remove.GetId(), page_to_remove.GetNumberOfPage());
           //std::cout << index_page << std::endl;
            free_index_on_ram_.insert(index_page);

            //Get a empty slot in the swapping area.
            int free_slot = GetFreeSlotOnSwapping();
            index_process_[page_to_remove.GetId()].EraseFromRam(index_page, free_slot);
        }
    }

    template<class T>
    void PCB<T>::AddProcess(int id_process, int number_of_pages) {
        Process process;
        for (int i = 0; i < number_of_pages; i++) {
            int free_slot = GetFreeSlotOnRam();
            process.SetIndexPageFromRam(i, free_slot);
            //std::cout << "METERE :" << id_process << " -> " << i << " " << cont_ << " " << free_slot << std::endl;
            //TODO: DELETE CONT.
            running_process_.insert(Page(id_process, i, ++cont_, false));
        }
        //std::cout << "Running process " << static_cast<int>(running_process_.size()) << std::endl;
        index_process_[id_process] = process;
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

        StatusOr<int> status = GetNumberOfPages(memory + 1);
        int page_number = status.GetData() - 1;

        //Check if it is on Ram.
        if (index_process_[id_process].OnRam(page_number)) {
            //std::cout << "Is on memory " << GetIndexPage(id_process, page_number);
        } else {
            std::string log = "The process " + std::to_string(id_process) + " : is not on memory. \n";
            return StatusOr<std::vector<std::string>>(Status::kError, log);
        }
        std::vector<std::string> information_process{"Good"};
        return StatusOr<std::vector<std::string>>(Status::kOk, information_process);
    }


} // virtual_memory


#endif //VIRTUAL_MEMORY_PCB_H
