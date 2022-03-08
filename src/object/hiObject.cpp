#include <stdio.h>
#include "object/hiObject.hpp"
#include "object/hiDict.hpp"
#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"

HiObject::HiObject() {
    set_klass(ObjectKlass::get_instance());
}

ObjectKlass* ObjectKlass::instance = nullptr;

ObjectKlass::ObjectKlass() {
    set_super(nullptr);
}

ObjectKlass* ObjectKlass::get_instance() {
    if (instance == nullptr) {
        instance = new ObjectKlass();
    }

    return instance;
}

void ObjectKlass::initialize() {
    set_klass_dict(new HiDict());
    (new HiTypeObject())->set_own_klass(this);
    set_name(new HiString("object"));
}

void ObjectKlass::print(HiObject* obj) {
    printf("<object object at 0x%lx>", obj);
}

HiObject* ObjectKlass::allocate_instance(ArrayList<HiObject*>* args) {
    return new HiObject();
}

void HiObject::print() {
    if (klass() == NativeFunctionKlass::get_instance()) {
        TypeKlass::get_instance()->type_object()->print();
    }

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

HiObject* HiObject::subscr(HiObject* x) {
    return klass()->subscr(this, x);
}

HiObject* HiObject::contains(HiObject* x) {
    return klass()->contains(this, x);
}

HiObject* HiObject::not_contains(HiObject* x) {
    return klass()->not_contains(this, x);
}

void HiObject::store_subscr(HiObject* x, HiObject* y) {
    klass()->store_subscr(this, x, y);
}

void HiObject::del_subscr(HiObject* x) {
    klass()->del_subscr(this, x);
}

HiObject* HiObject::unpack(HiObject* x) {
    return klass()->unpack(this, x);
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

HiObject* HiObject::iter() {
    return klass()->iter(this);
}

HiObject* HiObject::next() {
    return klass()->next(this);
}

/*
 * TypeObject is a special object.
 */
TypeKlass* TypeKlass::instance = nullptr;

TypeKlass* TypeKlass::get_instance() {
    if (instance == nullptr) {
        instance = new TypeKlass();
    }

    return instance;
}

void TypeKlass::initialize() {
    set_klass_dict(new HiDict());
    (new HiTypeObject())->set_own_klass(this);
    set_super(ObjectKlass::get_instance());
    set_name(new HiString("type"));
}

void TypeKlass::print(HiObject* obj) {
    assert(obj && obj->klass() == this);
    printf("<type ");
    Klass* own_klass = ((HiTypeObject*)obj)->own_klass();
    
    own_klass->name()->print();
    printf(">");
}

HiTypeObject::HiTypeObject() {
    set_klass(TypeKlass::get_instance());
}

void HiTypeObject::set_own_klass(Klass* k) {
    _own_klass = k;
    k->set_type_object(this);
}
