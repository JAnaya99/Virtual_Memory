#include "Page.h"

namespace virtual_memory {
    Page::Page(int id, int number_of_page, int timestamp, bool is_free) : id_(id), number_of_page_(number_of_page),
                                                                          timestamp_(timestamp), is_free_(is_free) {}

    int Page::GetId() const {
        return id_;
    }

    int Page::GetNumberOfPage() const {
        return number_of_page_;
    }

    int Page::GetTimestamp() const {
        return timestamp_;
    }

    int Page::GetArrivalNumber() const {
        return arrival_number_;
    }

    bool Page::IsFree() const {
        return is_free_;
    }
} // virtual_memory