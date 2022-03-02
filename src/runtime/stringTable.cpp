#include "runtime/stringTable.hpp"

StringTable* StringTable::instance = nullptr;

StringTable* StringTable::get_instance() {
    if (instance == nullptr) {
        instance = new StringTable();
    }

    return instance;
}

StringTable::StringTable() {
    next_str = new HiString("next");
}
