#include <stdio.h>
#include "object/hiList.hpp"
#include "object/hiInteger.hpp"
#include "runtime/universe.hpp"

HiList::HiList() {
    _inner_list = new ArrayList<HiObject*>();
    set_klass(ListKlass::get_instance());
}

HiList::HiList(ArrayList<HiObject*>*  ol) {
    _inner_list = ol;
    set_klass(ListKlass::get_instance());
}

ListKlass* ListKlass::instance = nullptr;

ListKlass* ListKlass::get_instance() {
    if (instance == nullptr) {
        instance = new ListKlass();
    }

    return instance;
}

void ListKlass::print(HiObject* obj) {
    HiList* lst_obj = (HiList*)obj;
    assert(lst_obj && lst_obj->klass() == this);

    printf("[");

    int size = lst_obj->inner_list()->size();
    if (size >= 1) {
        lst_obj->inner_list()->get(0)->print();
    }

    for (int i = 1; i < size; ++i) {
        printf(",");
        lst_obj->inner_list()->get(i)->print();
    }

    printf("]");
}

HiObject* ListKlass::subscr(HiObject* x, HiObject* y) {
    assert(x && x->klass() == this);
    assert(y && y->klass() == IntegerKlass::get_instance());

    HiList* lx = (HiList*)x;
    HiInteger* iy = (HiInteger*)y;

    return lx->inner_list()->get(iy->value());
}

HiObject* ListKlass::contains(HiObject* x, HiObject* y) {
    assert(x && x->klass() == this);
    
    HiList* lx = (HiList*)x;

    int size = lx->inner_list()->size();
    for (int i = 0; i < size; ++i) {
        if (lx->inner_list()->get(i)->equal(y) == Universe::HiTrue) {
            return Universe::HiTrue;
        }
    }

    return Universe::HiFalse;
}

HiObject* ListKlass::not_contains(HiObject* x, HiObject* y) {
    if (contains(x, y) == Universe::HiTrue) {
        return Universe::HiFalse;
    }
    else {
        return Universe::HiTrue;
    }
}
