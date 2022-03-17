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
    add_str = new HiString("__add__");
    sub_str = new HiString("__sub__");
    mul_str = new HiString("__mul__");
    div_str = new HiString("__div__");
    modulo_str = new HiString("__mod__");
    gt_str = new HiString("__gt__");
    lt_str = new HiString("__lt__");
    eq_str = new HiString("__eq__");
    ne_str = new HiString("__ne__");
    ge_str = new HiString("__ge__");
    le_str = new HiString("__le__");
    len_str = new HiString("__len__");
}
