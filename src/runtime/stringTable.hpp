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
    HiString* mod_str;
    HiString* object_str;
    HiString* type_str;
    HiString* int_str;
    HiString* str_str;
    HiString* list_str;
    HiString* dict_str;
};

#endif