#include "runtime/cellObject.hpp"
#include "object/hiString.hpp"
#include "object/hiDict.hpp"

CellKlass* CellKlass::instance = nullptr;

CellKlass* CellKlass::get_instance() {
    if (instance == nullptr) {
        instance = new CellKlass();
    }

    return instance;
}

CellKlass::CellKlass() {
    set_klass_dict(new HiDict());
    set_name(new HiString("cell"));
}

CellObject::CellObject(HiList* l, int i) : _table(l), _index(i) {
    set_klass(CellKlass::get_instance());
}

HiObject* CellObject::value() {
    return _table->get(_index);
}