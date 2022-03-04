#ifndef KLASS_HPP
#define KLASS_HPP

#include "util/arrayList.hpp"

class HiString;
class HiObject;
class HiDict;

class Klass {
private:
    Klass* _super;
    HiString* _name;
    HiDict* _klass_dict;

public:
    Klass() {}

    void set_super(Klass* super) { _super = super; }
    Klass* super() { return _super; }

    void set_name(HiString* x) { _name = x; }
    HiString* name() { return _name; }

    void set_klass_dict(HiDict* klass_dict) { _klass_dict = klass_dict; }
    HiDict* klass_dict() { return _klass_dict; }

    static int compare_klass(Klass* x, Klass* y);
    virtual void print(HiObject* obj) {}

    virtual HiObject* greater(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* less(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* equal(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* not_equal(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* ge(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* le(HiObject* x, HiObject* y) { return 0; }

    virtual HiObject* add(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* sub(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* mul(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* div(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* mod(HiObject* x, HiObject* y) { return 0; }

    virtual HiObject* len(HiObject* x) { return 0; }
    virtual HiObject* subscr(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* contains(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* not_contains(HiObject* x, HiObject* y) { return 0; }
    virtual void store_subscr(HiObject* x, HiObject* y, HiObject* z) { return; }
    virtual void del_subscr(HiObject* x, HiObject* y) { return; }
    virtual HiObject* iter(HiObject* x) { return 0; }
    virtual HiObject* call(ArrayList<HiObject*>* args) { return 0; }
};

#endif
