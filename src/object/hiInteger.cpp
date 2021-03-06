#include <stdio.h>
#include "object/hiInteger.hpp"
#include "object/hiDict.hpp"
#include "runtime/universe.hpp"

HiInteger::HiInteger(int x) {
    _value = x;
    set_klass(IntegerKlass::get_instance());
}

IntegerKlass* IntegerKlass::instance = nullptr;

IntegerKlass* IntegerKlass::get_instance() {
    if (instance == nullptr) {
        instance = new IntegerKlass();
    }

    return instance;
}

void IntegerKlass::initialize() {
    HiDict* klass_dict = new HiDict();
    set_klass_dict(klass_dict);

    (new HiTypeObject())->set_own_klass(this);
    add_super(ObjectKlass::get_instance());
    set_name(new HiString("int"));
}

void IntegerKlass::print(HiObject* obj) {
    HiInteger* int_obj = (HiInteger*)obj;

    assert(int_obj && (int_obj->klass() == this));

    printf("%d", int_obj->value());
}

HiObject* IntegerKlass::allocate_instance(HiObject* callable, ArrayList<HiObject*>* args) {
    if (!args || args->length() == 0) {
        return new HiInteger(0);
    }
    else {
        int value = ((HiInteger*)(args->get(0)))->value();
        return new HiInteger(value);
    }
}

HiObject* IntegerKlass::greater(HiObject* x, HiObject* y) {
    HiInteger* ix = (HiInteger*)x;
    HiInteger* iy = (HiInteger*)y;

    assert(ix && (ix->klass() == this));
    assert(ix && (iy->klass() == this));

    if (ix->value() > iy->value()) {
        return Universe::HiTrue;
    }
    else {
        return Universe::HiFalse;
    }
}

HiObject* IntegerKlass::less(HiObject* x, HiObject* y) {
    HiInteger* ix = (HiInteger*)x;
    assert(ix && (ix->klass() == this));

    if (x->klass() != y->klass()) {
        if (Klass::compare_klass(x->klass(), y->klass()) < 0) {
            return Universe::HiTrue;
        }
        else {
            return Universe::HiFalse;
        }
    }

    HiInteger* iy = (HiInteger*)y;

    if (ix->value() < iy->value()) {
        return Universe::HiTrue;
    }
    else {
        return Universe::HiFalse;
    }
}

HiObject* IntegerKlass::equal(HiObject* x, HiObject* y) {
    HiInteger* ix = (HiInteger*)x;
    assert(ix && (ix->klass() == this));

    if (x->klass() != y->klass()) {
        return Universe::HiFalse;
    }

    HiInteger* iy = (HiInteger*)y;

    if (ix->value() == iy->value()) {
        return Universe::HiTrue;
    }
    else {
        return Universe::HiFalse;
    }
}

HiObject* IntegerKlass::not_equal(HiObject* x, HiObject* y) {
    HiInteger* ix = (HiInteger*)x;
    assert(ix && (ix->klass() == this));

    if (x->klass() != y->klass()) {
        return Universe::HiFalse;
    }

    HiInteger* iy = (HiInteger*)y;
    
    if (ix->value() != iy->value()) {
        return Universe::HiTrue;
    }
    else {
        return Universe::HiFalse;
    }
}

HiObject* IntegerKlass::ge(HiObject* x, HiObject* y) {
    HiInteger* ix = (HiInteger*)x;
    HiInteger* iy = (HiInteger*)y;

    assert(ix && (ix->klass() == this));
    assert(iy && (iy->klass() == this));

    if (ix->value() >= iy->value()) {
        return Universe::HiTrue;
    }
    else {
        return Universe::HiFalse;
    }
}

HiObject* IntegerKlass::le(HiObject* x, HiObject* y) {
    HiInteger* ix = (HiInteger*)x;
    HiInteger* iy = (HiInteger*)y;

    assert(ix && (ix->klass() == this));
    assert(iy && (iy->klass() == this));

    if (ix->value() <= iy->value()) {
        return Universe::HiTrue;
    }
    else {
        return Universe::HiFalse;
    }
}

HiObject* IntegerKlass::add(HiObject* x, HiObject* y) {
    HiInteger* ix = (HiInteger*)x;
    HiInteger* iy = (HiInteger*)y;

    assert(ix && (ix->klass() == this));
    assert(iy && (iy->klass() == this));

    return new HiInteger(ix->value() + iy->value());
}

HiObject* IntegerKlass::sub(HiObject* x, HiObject* y) {
    HiInteger* ix = (HiInteger*)x;
    HiInteger* iy = (HiInteger*)y;

    assert(ix && (ix->klass() == this));
    assert(iy && (iy->klass() == this));

    return new HiInteger(ix->value() - iy->value());
}

HiObject* IntegerKlass::mul(HiObject* x, HiObject* y) {
    HiInteger* ix = (HiInteger*)x;
    HiInteger* iy = (HiInteger*)y;

    assert(ix && (ix->klass() == this));
    assert(iy && (iy->klass() == this));

    return new HiInteger(ix->value() * iy->value());
}

HiObject* IntegerKlass::div(HiObject* x, HiObject* y) {
    HiInteger* ix = (HiInteger*)x;
    HiInteger* iy = (HiInteger*)y;

    assert(ix && (ix->klass() == this));
    assert(iy && (iy->klass() == this));
    assert(iy->value() != 0);

    return new HiInteger(ix->value() / iy->value());
}

HiObject* IntegerKlass::mod(HiObject* x, HiObject* y) {
    HiInteger* ix = (HiInteger*)x;
    HiInteger* iy = (HiInteger*)y;

    assert(ix && (ix->klass() == this));
    assert(iy && (iy->klass() == this));
    assert(iy->value() != 0);

    return new HiInteger(ix->value() % iy->value());
}
