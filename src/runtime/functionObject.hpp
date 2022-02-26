#ifndef FUNCTION_OBJECT_HPP
#define FUNCTION_OBJECT_HPP

#include "object/hiObject.hpp"
#include "object/klass.hpp"
#include "code/codeObject.hpp"
#include "object/hiString.hpp"
#include "util/map.hpp"

class FunctionKlass : public Klass {
private:
    FunctionKlass() {}
    static FunctionKlass* instance;

public:
    static FunctionKlass* get_instance();

    virtual void print(HiObject* obj);
};

class FunctionObject : public HiObject {
friend class FunctionKlass;
friend class FrameObject;

private:
    CodeObject* _func_code;
    HiString* _func_name;
    Map<HiObject*, HiObject*>* _globals;
    ArrayList<HiObject*>* _defaults;

    unsigned int _flags;

public:
    FunctionObject(HiObject* code_object);
    FunctionObject(Klass* klass) {
        _func_code = nullptr;
        _func_name = nullptr;
        _globals = nullptr;
        _flags = 0;

        set_klass(klass);
    }

    HiString* func_name() { return _func_name; }
    
    Map<HiObject*, HiObject*>* globals() { return _globals; }
    void set_globals(Map<HiObject*, HiObject*>* x) { _globals = x; }
    
    ArrayList<HiObject*>* defaults() { return _defaults; }
    void set_defaults(ArrayList<HiObject*>* defaults);

    int flags() { return _flags; }
};

#endif