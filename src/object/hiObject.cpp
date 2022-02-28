#include "object/hiObject.hpp"
#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"

void HiObject::print() {
    klass()->print(this);
}

HiObject* HiObject::greater(HiObject* rhs) {
    return klass()->greater(this, rhs);
}

HiObject* HiObject::less(HiObject* rhs) {
    return klass()->less(this, rhs);
}

HiObject* HiObject::equal(HiObject* rhs) {
    return klass()->equal(this, rhs);
}

HiObject* HiObject::not_equal(HiObject* rhs) {
    return klass()->not_equal(this, rhs);
}

HiObject* HiObject::ge(HiObject* rhs) {
    return klass()->ge(this, rhs);
}

HiObject* HiObject::le(HiObject* rhs) {
    return klass()->le(this, rhs);
}

HiObject* HiObject::add(HiObject* rhs) {
    return klass()->add(this, rhs);
}

HiObject* HiObject::sub(HiObject* rhs) {
    return klass()->sub(this, rhs);
}

HiObject* HiObject::mul(HiObject* rhs) {
    return klass()->mul(this, rhs);
}

HiObject* HiObject::div(HiObject* rhs) {
    return klass()->div(this, rhs);
}

HiObject* HiObject::mod(HiObject* rhs) {
    return klass()->mod(this, rhs);
}

HiObject* HiObject::len() {
    return klass()->len(this);
}

HiObject* HiObject::getattr(HiObject* x) {
    HiObject* result = klass()->klass_dict()->get(x);
    if (result == Universe::HiNone) {
        return result;
    }

    if (MethodObject::is_function(result)) {
        result = new MethodObject(this, (FunctionObject*)result);
    }
    return result;
}