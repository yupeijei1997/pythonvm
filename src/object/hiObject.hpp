#ifndef HI_OBJECT_HPP
#define HI_OBJECT_HPP

#include <assert.h>
#include "object/klass.hpp"

class HiObject {
private:
    Klass* _klass;

public:
    Klass* klass() { assert(_klass != nullptr); return _klass; }
    void set_klass(Klass* klass) { _klass = klass; }

    virtual void print();

    HiObject* greater(HiObject* x);
    HiObject* less(HiObject* x);
    HiObject* equal(HiObject* x);
    HiObject* not_equal(HiObject* x);
    HiObject* ge(HiObject* x);
    HiObject* le(HiObject* x);

    HiObject* add(HiObject* x);
    HiObject* sub(HiObject* x);
    HiObject* mul(HiObject* x);
    HiObject* div(HiObject* x);
    HiObject* mod(HiObject* x);

    HiObject* len();
    HiObject* subscr(HiObject* x);
    HiObject* contains(HiObject* x);
    HiObject* not_contains(HiObject* x);
    void store_subscr(HiObject* x, HiObject* y);

    HiObject* getattr(HiObject* x);
};

#endif