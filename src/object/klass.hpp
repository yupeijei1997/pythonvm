#ifndef KLASS_HPP
#define KLASS_HPP

#include "util/arrayList.hpp"
#include "util/map.hpp"

class HiString;
class HiObject;

class Klass {
private:
    Klass* _super;
    HiString* _name;
    Map<HiObject*, HiObject*>* _klass_dict;

public:
    Klass() {}

    void set_super(Klass* super) { _super = super; }
    Klass* super() { return _super; }

    void set_name(HiString* x) { _name = x; }
    HiString* name() { return _name; }

    void set_klass_dict(Map<HiObject*, HiObject*>* klass_dict) { _klass_dict = klass_dict; }
    Map<HiObject*, HiObject*>* klass_dict() { return _klass_dict; }

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
    virtual HiObject* call(ArrayList<HiObject*>* args) { return 0; }
};

#endif
