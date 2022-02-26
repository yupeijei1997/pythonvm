#ifndef HI_STRING_HPP
#define HI_STRING_HPP

#include "object/hiObject.hpp"
#include "object/klass.hpp"

class StringKlass : public Klass {
private:
    StringKlass() {}
    static StringKlass* instance;

public:
    static StringKlass* get_instance();

    virtual HiObject* equal(HiObject* x, HiObject* y);

    virtual void print(HiObject* obj);
};

class HiString : public HiObject {
private:
    char* _value;
    int _length;

public:
    HiString(const char* x);
    HiString(const char* x, int length);
    ~HiString();

    const char* value() { return _value; }
    int length() { return _length; }
};

#endif