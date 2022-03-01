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

class NativeFunctionKlass : public Klass {
private:
    NativeFunctionKlass();
    static NativeFunctionKlass* instance;

public:
    static NativeFunctionKlass* get_instance();
};

HiObject* len(ArrayList<HiObject*>* args);

typedef HiObject* (*NativeFuncPointer)(ArrayList<HiObject*>* args);

class FunctionObject : public HiObject {
friend class FunctionKlass;
friend class FrameObject;

private:
    CodeObject* _func_code;
    HiString* _func_name;
    Map<HiObject*, HiObject*>* _globals;
    ArrayList<HiObject*>* _defaults;

    NativeFuncPointer _native_func;

    unsigned int _flags;

public:
    FunctionObject(HiObject* code_object);
    FunctionObject(NativeFuncPointer nfp);
    FunctionObject(Klass* klass) {
        _func_code = nullptr;
        _func_name = nullptr;
        _globals = nullptr;
        _flags = 0;

        set_klass(klass);
    }


    HiString* func_name() { return _func_name; }
    int flags() { return _flags; }

    Map<HiObject*, HiObject*>* globals() { return _globals; }
    void set_globals(Map<HiObject*, HiObject*>* x) { _globals = x; }
    
    ArrayList<HiObject*>* defaults() { return _defaults; }
    void set_defaults(ArrayList<HiObject*>* defaults);

    HiObject* call(ArrayList<HiObject*>* args);
};

class MethodKlass : public Klass {
private:
    MethodKlass();
    static MethodKlass* instance;

public:
    static MethodKlass* get_instance();
};

class MethodObject : public HiObject {
friend class MethodKlass;

private:
    HiObject* _owner;
    FunctionObject* _func;

public:
    MethodObject(FunctionObject* func) : _owner(nullptr), _func(func) {
        set_klass(MethodKlass::get_instance());
    }

    MethodObject(HiObject* owner, FunctionObject* func) : _owner(owner), _func(func) {
        set_klass(MethodKlass::get_instance());
    }

    void set_owner(HiObject* owner) { _owner = owner; }
    HiObject* owner() { return _owner; }
    FunctionObject* func() { return _func; }

    static bool is_function(HiObject* x);
};

#endif