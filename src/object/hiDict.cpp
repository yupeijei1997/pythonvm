#include <stdio.h>
#include <assert.h>
#include "object/hiDict.hpp"
#include "object/hiObject.hpp"
#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"

HiDict::HiDict() {
    _map = new Map<HiObject*, HiObject*>();
    set_klass(DictKlass::get_instance());
}

HiDict::HiDict(Map<HiObject*, HiObject*>* map) {
    _map = map;
    set_klass(DictKlass::get_instance());
}

DictKlass* DictKlass::instance = nullptr;

DictKlass* DictKlass::get_instance() {
    if (instance == nullptr) {
        instance = new DictKlass();
    }

    return instance;
}

void DictKlass::initialize() {
    HiDict* klass_dict = new HiDict();
    klass_dict->put(new HiString("setdefault"), new FunctionObject(dict_set_default));
    klass_dict->put(new HiString("remove"), new FunctionObject(dict_remove));
    set_klass_dict(klass_dict);
}

void DictKlass::print(HiObject* obj) {
    HiDict* dict_obj = (HiDict*)obj;
    assert(dict_obj && dict_obj->klass() == this);

    printf("{");
    int size = dict_obj->size();
    if (size >= 1) {
        dict_obj->_map->entries()[0]._k->print();
        printf(":");
        dict_obj->_map->entries()[0]._v->print();
    }

    for (int i = 1; i < size; ++i) {
        printf(",");
        dict_obj->_map->entries()[i]._k->print();
        printf(":");
        dict_obj->_map->entries()[i]._v->print();
    }

    printf("}");
}

HiObject* DictKlass::subscr(HiObject* x, HiObject* y) {
    assert(x && x->klass() == this);
    return ((HiDict*)x)->get(y);
}

void DictKlass::store_subscr(HiObject* x, HiObject* y, HiObject* z) {
    assert(x && x->klass() == this);
    ((HiDict*)x)->put(y, z);
}

void DictKlass::del_subscr(HiObject* x, HiObject* y) {
    assert(x && x->klass() == this);
    ((HiDict*)x)->remove(y);
}

HiObject* dict_set_default(ArrayList<HiObject*>* args) {
    HiDict* dict = (HiDict*)(args->get(0));
    HiObject* key = args->get(1);
    HiObject* value = args->get(2);

    if (!dict->has_key(key)) {
        dict->put(key, value);
    }

    return Universe::HiNone;
}

HiObject* dict_remove(ArrayList<HiObject*>* args) {
    HiDict* x = (HiDict*)(args->get(0));
    HiObject* y = args->get(1);

    x->remove(y);
}
