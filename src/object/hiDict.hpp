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
    bool has_key(HiObject* k) { return _map->has_key(k); }
    int size() { return _map->size(); }
    HiObject* remove(HiObject* k) { return _map->remove(k); }
};

HiObject* dict_set_default(ArrayList<HiObject*>* args);
HiObject* dict_remove(ArrayList<HiObject*>* args);

#endif