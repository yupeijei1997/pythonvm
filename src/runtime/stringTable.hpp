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
    HiString* init_str;
    HiString* add_str;
    HiString* sub_str;
    HiString* mul_str;
    HiString* div_str;
    HiString* modulo_str;
    HiString* gt_str;
    HiString* lt_str;
    HiString* eq_str;
    HiString* ne_str;
    HiString* ge_str;
    HiString* le_str;
    HiString* len_str;
};

#endif