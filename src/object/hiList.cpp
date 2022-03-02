#include <stdio.h>
#include "object/hiList.hpp"
#include "object/hiInteger.hpp"
#include "object/hiString.hpp"
#include "runtime/functionObject.hpp"
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

ListKlass::ListKlass() {
    Map<HiObject*, HiObject*>* klass_dict = new Map<HiObject*, HiObject*>();
    klass_dict->put(new HiString("append"), new FunctionObject(list_append));
    klass_dict->put(new HiString("insert"), new FunctionObject(list_insert));
    klass_dict->put(new HiString("index"), new FunctionObject(list_index));
    klass_dict->put(new HiString("pop"), new FunctionObject(list_pop));
    klass_dict->put(new HiString("remove"), new FunctionObject(list_remove));
    klass_dict->put(new HiString("reverse"), new FunctionObject(list_reverse));
    klass_dict->put(new HiString("sort"), new FunctionObject(list_sort));
    set_klass_dict(klass_dict);
    set_name(new HiString("list"));
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

HiObject* ListKlass::less(HiObject* x, HiObject* y) {
    HiList* lx = (HiList*)x;
    assert(lx && lx->klass() == this);

    if (x->klass() != y->klass()) {
        if (Klass::compare_klass(x->klass(), y->klass()) < 0) {
            return Universe::HiTrue;
        }
        else {
            return Universe::HiFalse;
        }
    }

    HiList* ly = (HiList*)y;

    int len = lx->size() < ly->size() ? lx->size() : ly->size();
    for (int i = 0; i < len; ++i) {
        if (lx->get(i)->less(ly->get(i)) == Universe::HiTrue) {
            return Universe::HiTrue;
        }
        else {
            return Universe::HiFalse;
        }
    }

    if (lx->size() < ly->size()) {
        return Universe::HiTrue;
    }

    return Universe::HiFalse;
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

void ListKlass::store_subscr(HiObject* x, HiObject* y, HiObject* z) {
    assert(x && x->klass() == this);
    assert(y && y->klass() == IntegerKlass::get_instance());

    HiList* lx = (HiList*)x;
    HiInteger* iy = (HiInteger*)y;

    lx->inner_list()->set(iy->value(), z);
}

void ListKlass::del_subscr(HiObject* x, HiObject* y) {
    assert(x && x->klass() == this);
    assert(y && y->klass() == IntegerKlass::get_instance());

    HiList* lx = (HiList*)x;
    HiInteger* iy = (HiInteger*)y;

    lx->delete_index(iy->value());
}

HiObject* list_append(ArrayList<HiObject*>* args) {
    ((HiList*)(args->get(0)))->append(args->get(1));
    return Universe::HiNone;
}

HiObject* list_insert(ArrayList<HiObject*>* args) {
    ((HiList*)(args->get(0)))->insert(((HiInteger*)(args->get(1)))->value(), args->get(2));
    return Universe::HiNone;
}

HiObject* list_index(ArrayList<HiObject*>* args) {
    HiList* list = (HiList*)(args->get(0));
    HiObject* target = args->get(1);

    assert(list && list->klass() == ListKlass::get_instance());

    int size = list->size();
    for (int i = 0; i < size; ++i) {
        if (list->get(i)->equal(target) == Universe::HiTrue) {
            return new HiInteger(i);
        }
    }

    return Universe::HiNone;
}

HiObject* list_pop(ArrayList<HiObject*>* args) {
    HiList* list = (HiList*)(args->get(0));
    assert(list && list->klass() == ListKlass::get_instance());
    return list->pop();
}

HiObject* list_remove(ArrayList<HiObject*>* args) {
    HiList* list = (HiList*)(args->get(0));
    HiObject* target = args->get(1);

    assert(list && list->klass() == ListKlass::get_instance());

    int size = list->size();
    for (int i = 0; i < size; ++i) {
        if (list->get(i)->equal(target) == Universe::HiTrue) {
            list->delete_index(i);
            return Universe::HiNone;
        }
    }
    return Universe::HiNone; 
}

HiObject* list_reverse(ArrayList<HiObject*>* args) {
    HiList* list = (HiList*)(args->get(0));
    assert(list && list->klass() == ListKlass::get_instance());

    int i = 0, j = list->size() - 1;
    while (i < j) {
        HiObject* tmp = list->get(i);
        list->set(i, list->get(j));
        list->set(j, tmp);

        ++i;
        --j;
    }
    return Universe::HiNone;
}

HiObject* list_sort(ArrayList<HiObject*>* args) {
    HiList* list = (HiList*)(args->get(0));
    assert(list && list->klass() == ListKlass::get_instance());

    // bubble sort
    int size = list->size();
    for (int i = 0; i < size; ++i) {
        for (int j = size - 1; j > i; --j) {
            if (list->get(j)->less(list->get(j - 1)) == Universe::HiTrue) {
                HiObject* tmp = list->get(j - 1);
                list->set(j - 1, list->get(j));
                list->set(j, tmp);
            }
        }
    }
    return Universe::HiNone;
}
