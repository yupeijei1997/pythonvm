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

HiObject* StringKlass::len(HiObject* obj) {
    return new HiInteger(((HiString*)obj)->length());
}

HiObject* StringKlass::subscr(HiObject* x, HiObject* y) {
    assert(x && x->klass() == this);
    assert(y && y->klass() == IntegerKlass::get_instance());

    HiString* sx = (HiString*)x;
    HiInteger* iy = (HiInteger*)y;

    return new HiString(&(sx->value()[iy->value()]), 1);
}

HiObject* StringKlass::contains(HiObject* x, HiObject* y) {
    assert(x && x->klass() == this);
    assert(y && y->klass() == this);

    HiString* sx = (HiString*)x;
    HiString* sy = (HiString*)y;

    int sx_size = sx->length();
    int sy_size = sy->length();

    if (sy_size == 0) {
        return Universe::HiTrue;
    }

    // 1. 求 next 数组，即模式串 p 的各个子串的最长公共前后缀的长度
    ArrayList<int>* next = new ArrayList<int>(sy_size);
    next->set(0, 0);
    for (int j = 0, i = 1; i < sy_size; ++i) {
        while (j > 0 && sy->value()[j] != sy->value()[i]) {
            j = next->get(j - 1);
        }
        if (sy->value()[j] == sy->value()[i]) {
            ++j;
        }
        next->set(i, j);
    }

    // 2. 主串 s 和模式串 p 进行匹配
    for (int i = 0, j = 0; i < sx_size; ++i) {
        while (j > 0 && sx->value()[i] != sy->value()[j]) {
            j = next->get(j - 1);
        }
        if (sx->value()[i] == sy->value()[j]) {
            ++j;
        }
        if (j == sy_size) {
            return Universe::HiTrue;
        }
    }

    return Universe::HiFalse;
}

HiObject* StringKlass::not_contains(HiObject* x, HiObject* y) {
    if (contains(x, y) == Universe::HiTrue) {
        return Universe::HiFalse;
    }
    else {
        return Universe::HiTrue;
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

HiObject* string_upper(ArrayList<HiObject*>* args) {
    HiObject* arg0 = args->get(0);
    assert(arg0->klass() == StringKlass::get_instance());

    HiString* str_obj = (HiString*)arg0;
    int length = str_obj->length();
    if (length <= 0) {
        return Universe::HiNone;
    }

    char* v = new char[length];
    char c;
    for (int i = 0; i < length; ++i) {
        c = str_obj->value()[i];
        // convert to upper
        if (c >= 'a' && c <= 'z') {
            v[i] = c - 0x20;
        }
        else {
            v[i] = c;
        }
    }

    HiString* s = new HiString(v, length);
    delete[] v;
    return s;
}
