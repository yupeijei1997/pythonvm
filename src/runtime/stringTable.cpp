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
    mod_str = new HiString("__module__");
    object_str = new HiString("object");
    type_str = new HiString("type");
    int_str = new HiString("int");
    str_str = new HiString("str");
    list_str = new HiString("list");
    dict_str = new HiString("dict");  
    init_str = new HiString("__init__");
}
