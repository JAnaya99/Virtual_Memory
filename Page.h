#ifndef VIRTUAL_MEMORY_PAGE_H
#define VIRTUAL_MEMORY_PAGE_H

namespace virtual_memory {
    //This class is the basic unit of the RAM and swapping area.
    class Page {
    public:
        Page() = default;

        //Constructors.
        Page(int id, int number_of_page, int timestamp, int arrival_number, bool is_free);

        //Getters.
        int GetId() const;

        int GetNumberOfPage() const;

        int GetTimestamp() const;

        bool IsFree() const;

        int GetArrivalNumber() const;

    private:
        int id_;
        int number_of_page_;
        int timestamp_;
        int arrival_number_;
        bool is_free_;
    };
} // virtual_memory


#endif //VIRTUAL_MEMORY_PAGE_H
