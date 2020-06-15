#ifndef VIRTUAL_MEMORY_REPLACEMENT_ALGORITHMS_H
#define VIRTUAL_MEMORY_REPLACEMENT_ALGORITHMS_H

#include "Page.h"


namespace virtual_memory {
    // This namespace contains the replacement algorithm policies.
    // To add other algorithm create a class and overload the operator () creating a custom comparator.
    namespace replacement_algorithms {
        //Custom comparator for FIFO. Arrival time matter.
        class FIFO {
        public:
            bool operator()(const Page &a, const Page &b) {
                return a.GetArrivalNumber() < b.GetArrivalNumber();
            }
        };

        //Custom comparator for LRU. Last timestamp matter.
        class LRU {
        public:
            bool operator()(const Page &a, const Page &b) {
                return a.GetTimestamp() < b.GetTimestamp();
            }
        };
    } // replacement_algorithms
} // virtual_memory

#endif //VIRTUAL_MEMORY_REPLACEMENT_ALGORITHMS_H
