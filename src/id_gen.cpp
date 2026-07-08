#include "OrderBook.hpp"

namespace ID {
    size_t get_unique_id() {
        static size_t id {};
        id++;
        return id + 1000;
    }
}

