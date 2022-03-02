#ifndef STRING_TABLE_HPP
#define STRING_TABLE_HPP

#include "object/hiString.hpp"

class StringTable {
private:
    StringTable();
    static StringTable* instance;

public:
    static StringTable* get_instance();
    HiString* next_str;
};

#endif