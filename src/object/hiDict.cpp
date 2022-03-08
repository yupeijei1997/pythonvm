#include <stdio.h>
#include <assert.h>
#include "object/hiDict.hpp"
#include "object/hiObject.hpp"
#include "object/hiList.hpp"
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
    klass_dict->put(new HiString("keys"), new FunctionObject(dict_keys));
    klass_dict->put(new HiString("values"), new FunctionObject(dict_values));
    klass_dict->put(new HiString("items"), new FunctionObject(dict_items));
    klass_dict->put(new HiString("iterkeys"), new FunctionObject(dict_iterkeys));
    klass_dict->put(new HiString("itervalues"), new FunctionObject(dict_itervalues));
    klass_dict->put(new HiString("iteritems"), new FunctionObject(dict_iteritems));
    set_klass_dict(klass_dict);

    (new HiTypeObject())->set_own_klass(this);
    set_super(ObjectKlass::get_instance());
    set_name(new HiString("dict"));
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

HiObject* DictKlass::allocate_instance(ArrayList<HiObject*>* args) {
    if (!args || args->length() == 0) {
        return new HiDict();
    }
    else {
        return args->get(0);
    }
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

HiObject* DictKlass::unpack(HiObject* x, HiObject* y) {
    assert(x && x->klass() == this);
    assert(y && y->klass() == IntegerKlass::get_instance());

    HiDict* dx = (HiDict*)x;
    HiInteger* iy = (HiInteger*)y;

    return dx->get_key(iy->value());
}

HiObject* DictKlass::iter(HiObject* x) {
    HiObject* it = new DictIterator((HiDict*)x);
    it->set_klass(DictIteratorKlass<ITER_KEY>::get_instance());
    return it;
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
    return Universe::HiNone;
}

HiObject* dict_keys(ArrayList<HiObject*>* args) {
    HiDict* x = (HiDict*)(args->get(0));

    HiList* keys = new HiList();

    for (int i = 0; i < x->size(); ++i) {
        keys->append(x->get_key(i));
    }
    return keys;
}

HiObject* dict_values(ArrayList<HiObject*>* args) {
    HiDict* x = (HiDict*)(args->get(0));

    HiList* values = new HiList();

    for (int i = 0; i < x->size(); ++i) {
        values->append(x->get_value(i));
    }
    return values;
}

HiObject* dict_items(ArrayList<HiObject*>* args) {
    HiDict* x = (HiDict*)(args->get(0));

    HiList* items = new HiList();

    for (int i = 0; i < x->size(); ++i) {
        HiList* item = new HiList();
        item->append(x->get_key(i));
        item->append(x->get_value(i));
        items->append(item);
    }
    return items;
}

/*
 * Iterations for dict object
 */
template<ITER_TYPE n>
DictIteratorKlass<n>* DictIteratorKlass<n>::instance = nullptr;

template<ITER_TYPE n>
DictIteratorKlass<n>* DictIteratorKlass<n>::get_instance() {
    if (instance == nullptr) {
        instance = new DictIteratorKlass();
    }

    return instance;
}

template<ITER_TYPE iter_type>
DictIteratorKlass<iter_type>::DictIteratorKlass() {
    const char* klass_names[] = {
        "dictionary-keyiterator",
        "dictionary-valueiterator",
        "dictionary-itemiterator"
    };
    set_klass_dict(new HiDict());
    set_name(new HiString(klass_names[iter_type]));
}

DictIterator::DictIterator(HiDict* owner) {
    _owner = owner;
    _iter_cnt = 0;
}

HiObject* dict_iterkeys(ArrayList<HiObject*>* args) {
    HiDict* x = (HiDict*)(args->get(0));
    HiObject* it = new DictIterator(x);
    it->set_klass(DictIteratorKlass<ITER_KEY>::get_instance());
    return it;
}

HiObject* dict_itervalues(ArrayList<HiObject*>* args) {
    HiDict* x = (HiDict*)(args->get(0));
    HiObject* it = new DictIterator(x);
    it->set_klass(DictIteratorKlass<ITER_VALUE>::get_instance());
    return it;
}

HiObject* dict_iteritems(ArrayList<HiObject*>* args) {
    HiDict* x = (HiDict*)(args->get(0));
    HiObject* it = new DictIterator(x);
    it->set_klass(DictIteratorKlass<ITER_ITEM>::get_instance());
    return it;
}

template<ITER_TYPE iter_type>
HiObject* DictIteratorKlass<iter_type>::next(HiObject* x) {
    DictIterator* iter = (DictIterator*)x;

    HiDict* adict = iter->owner();
    int iter_cnt = iter->iter_cnt();
    if (iter_cnt < adict->size()) {
        HiObject* obj;
        if (iter_type == ITER_KEY) {
            obj = adict->get_key(iter_cnt);
        }
        else if (iter_type == ITER_VALUE){
            obj = adict->get_value(iter_cnt);
        }
        else {
            HiList* lobj = new HiList();
            lobj->append(adict->get_key(iter_cnt));
            lobj->append(adict->get_value(iter_cnt));
            obj = lobj;
        }
        iter->inc_cnt();
        return obj;
    }
    else {
        return nullptr;
    }
}
