#include <stdio.h>
#include <string.h>
#include "object/hiString.hpp"
#include "runtime/universe.hpp"

StringKlass* StringKlass::instance = nullptr;

StringKlass* StringKlass::get_instance() {
    if (instance == nullptr) {
        instance = new StringKlass();
    }
    else {
        return instance;
    }
}

HiObject* StringKlass::equal(HiObject* x, HiObject* y) {
    HiString* sx = (HiString*)x;
    HiString* sy = (HiString*)y;

    assert(sx && (sx->klass() == this));
    assert(sy && (sy->klass() == this));

    if (sx->length() != sy->length()) {
        return Universe::HiFalse;
    }

    for (int i = 0; i < sx->length(); ++i) {
        if (sx->value()[i] != sy->value()[i]) {
            return Universe::HiFalse;
        }
    }

    return Universe::HiTrue;
}

void StringKlass::print(HiObject* obj) {
    HiString* str_obj = (HiString*)obj;

    assert(str_obj && (str_obj->klass() == this));

    for (int i = 0; i < str_obj->length(); ++i) {
        printf("%c", str_obj->value()[i]);
    }
}


HiString::HiString(const char* x) {
    _length = strlen(x);
    _value = new char[_length];
    strcpy(_value, x);

    set_klass(StringKlass::get_instance());
}

HiString::HiString(const char* x, int length) {
    _length = length;
    _value = new char[length];
    // Do not use strcpy here, because '\0' is allowed.
    for (int i = 0; i < length; ++i) {
        _value[i] = x[i];
    }

    set_klass(StringKlass::get_instance());
}

HiString::~HiString() {
    delete[] _value;
    _value = nullptr;
}
