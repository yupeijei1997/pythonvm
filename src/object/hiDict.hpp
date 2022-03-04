#ifndef HI_DICT_HPP
#define HI_DICT_HPP

#include "object/hiObject.hpp"
#include "util/map.hpp"

class DictKlass : public Klass {
private:
    DictKlass() {}
    static DictKlass* instance;

public:
    static DictKlass* get_instance();
    void initialize();

    virtual void print(HiObject* obj);
    virtual HiObject* subscr(HiObject* x, HiObject* y);
    virtual void store_subscr(HiObject* x, HiObject* y, HiObject* z);
    virtual void del_subscr(HiObject* x, HiObject* y);
    virtual HiObject* unpack(HiObject* x, HiObject* y);
    virtual HiObject* iter(HiObject* x);
};

class HiDict : public HiObject {
friend class DictKlass;

private:
    Map<HiObject*, HiObject*>* _map;

public:
    HiDict();
    HiDict(Map<HiObject*, HiObject*>* map);

    Map<HiObject*, HiObject*>* map() { return _map; }
    void put(HiObject* k, HiObject* v) { _map->put(k, v); }
    HiObject* get(HiObject* k) { return _map->get(k); }
    HiObject* get_key(int i) { return _map->get_key(i); }
    HiObject* get_value(int i) { return _map->get_value(i); }
    bool has_key(HiObject* k) { return _map->has_key(k); }
    int size() { return _map->size(); }
    HiObject* remove(HiObject* k) { return _map->remove(k); }
};

HiObject* dict_set_default(ArrayList<HiObject*>* args);
HiObject* dict_remove(ArrayList<HiObject*>* args);
HiObject* dict_keys(ArrayList<HiObject*>* args);
HiObject* dict_values(ArrayList<HiObject*>* args);
HiObject* dict_items(ArrayList<HiObject*>* args);

enum ITER_TYPE {
    ITER_KEY = 0,
    ITER_VALUE,
    ITER_ITEM
};

template<ITER_TYPE n>
class DictIteratorKlass : public Klass {
private:
    DictIteratorKlass();
    static DictIteratorKlass* instance;

public:
    static DictIteratorKlass* get_instance();
    virtual HiObject* iter(HiObject* x) { return x; }
    virtual HiObject* next(HiObject* x);
};

class DictIterator : public HiObject {
private:
    HiDict* _owner;
    int _iter_cnt;

public:
    DictIterator(HiDict* owner);

    HiDict* owner() { return _owner; }
    int iter_cnt() { return _iter_cnt; }
    void inc_cnt() { ++_iter_cnt; }
};

HiObject* dict_iterkeys(ArrayList<HiObject*>* args);
HiObject* dict_itervalues(ArrayList<HiObject*>* args);
HiObject* dict_iteritems(ArrayList<HiObject*>* args);

#endif