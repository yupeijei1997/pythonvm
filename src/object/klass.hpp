#ifndef KLASS_HPP
#define KLASS_HPP

#include "util/arrayList.hpp"

class HiTypeObject;
class HiString;
class HiObject;
class HiDict;
class HiList;

class Klass {
private:
    HiList* _super;
    HiTypeObject* _type_object;
    HiString* _name;
    HiDict* _klass_dict;

public:
    Klass() {}

    static HiObject* create_klass(HiObject* x, HiObject* supers, HiObject* name);

    void add_super(Klass* x);
    HiTypeObject* super();
    void set_super_list(HiList* x) { _super = x; }

    void set_type_object(HiTypeObject* x) { _type_object = x; }
    HiTypeObject* type_object() { return _type_object; }

    void set_name(HiString* x) { _name = x; }
    HiString* name() { return _name; }

    void set_klass_dict(HiDict* klass_dict) { _klass_dict = klass_dict; }
    HiDict* klass_dict() { return _klass_dict; }

    static int compare_klass(Klass* x, Klass* y);
    virtual void print(HiObject* obj);

    virtual HiObject* greater(HiObject* x, HiObject* y);
    virtual HiObject* less(HiObject* x, HiObject* y);
    virtual HiObject* equal(HiObject* x, HiObject* y);
    virtual HiObject* not_equal(HiObject* x, HiObject* y);
    virtual HiObject* ge(HiObject* x, HiObject* y);
    virtual HiObject* le(HiObject* x, HiObject* y);

    virtual HiObject* add(HiObject* x, HiObject* y);
    virtual HiObject* sub(HiObject* x, HiObject* y);
    virtual HiObject* mul(HiObject* x, HiObject* y);
    virtual HiObject* div(HiObject* x, HiObject* y);
    virtual HiObject* mod(HiObject* x, HiObject* y);

    virtual HiObject* find_and_call(HiObject* x, ArrayList<HiObject*>* args, HiObject* func_name);
    virtual HiObject* allocate_instance(HiObject* callable, ArrayList<HiObject*>* args);

    virtual HiObject* len(HiObject* x);
    virtual HiObject* subscr(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* contains(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* not_contains(HiObject* x, HiObject* y) { return 0; }
    virtual void store_subscr(HiObject* x, HiObject* y, HiObject* z) { return; }
    virtual void del_subscr(HiObject* x, HiObject* y) { return; }
    virtual HiObject* unpack(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* iter(HiObject* x) { return 0; }
    virtual HiObject* next(HiObject* x) { return 0; }
    virtual HiObject* call(ArrayList<HiObject*>* args) { return 0; }
    virtual HiObject* getattr(HiObject* x, HiObject* y);
    virtual HiObject* setattr(HiObject* x, HiObject* y, HiObject* z);
};

#endif
