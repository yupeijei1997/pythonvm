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
    void initialize();

    virtual HiObject* equal(HiObject* x, HiObject* y);

    virtual void print(HiObject* obj);

    virtual HiObject* allocate_instance(ArrayList<HiObject*>* args);

    virtual HiObject* less(HiObject* x, HiObject* y);
    virtual HiObject* len(HiObject* obj);
    virtual HiObject* subscr(HiObject* x, HiObject* y);
    virtual HiObject* contains(HiObject* x, HiObject* y);
    virtual HiObject* not_contains(HiObject* x, HiObject* y);
};

class HiString : public HiObject {
private:
    char* _value;
    int _length;

public:
    HiString(const char* x);
    HiString(const char* x, int length);

    const char* value() { return _value; }
    char* value2() { return _value; }
    int length() { return _length; }
};

HiObject* string_upper(ArrayList<HiObject*>* args);

#endif