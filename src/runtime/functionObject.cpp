#include <stdio.h>
#include <assert.h>
#include "runtime/functionObject.hpp"
#include "runtime/universe.hpp"
#include "object/hiDict.hpp"

FunctionKlass* FunctionKlass::instance = nullptr;

FunctionKlass* FunctionKlass::get_instance() {
    if (instance == nullptr) {
        instance = new FunctionKlass();
    }

    return instance;
}

void FunctionKlass::print(HiObject* obj) {
    printf("<function : ");
    FunctionObject* fo = (FunctionObject*)obj;

    assert(fo && (fo->klass() == this));
    fo->func_name()->print();
    printf(">");
}

FunctionObject::FunctionObject(HiObject* code_object) {
    CodeObject* co = (CodeObject*)code_object;

    _func_code = co;
    _func_name = co->_co_name;
    _flags = co->_flag;
    _globals = nullptr;

    set_klass(FunctionKlass::get_instance());
}

FunctionObject::FunctionObject(NativeFuncPointer nfp) {
    _func_code = nullptr;
    _func_name = nullptr;
    _flags = 0;
    _globals = nullptr;
    _native_func = nfp;

    set_klass(NativeFunctionKlass::get_instance());
}

void FunctionObject::set_defaults(ArrayList<HiObject*>* defaults) {
    if (defaults == nullptr) {
        _defaults = nullptr;
        return;
    }

    _defaults = new ArrayList<HiObject*>(defaults->length());
    for (int i = 0; i < defaults->length(); ++i) {
        _defaults->set(i, defaults->get(i));
    }
}

/*
 * Operations for native calls.
 */

NativeFunctionKlass* NativeFunctionKlass::instance = nullptr;

NativeFunctionKlass* NativeFunctionKlass::get_instance() {
    if (instance == nullptr) {
        instance = new NativeFunctionKlass();
    }

    return instance;
}

NativeFunctionKlass::NativeFunctionKlass() {
    set_super(FunctionKlass::get_instance());
}

HiObject* len(ArrayList<HiObject*>* args) {
    return args->get(0)->len();
}

HiObject* isinstance(ArrayList<HiObject*>* args) {
    HiObject* x = args->get(0);
    HiObject* y = args->get(1);

    if (x->klass() == NativeFunctionKlass::get_instance()) {
        x = TypeKlass::get_instance()->type_object();
    }

    if (y->klass() == NativeFunctionKlass::get_instance()) {
        y = TypeKlass::get_instance()->type_object();
    }

    assert(y && y->klass() == TypeKlass::get_instance());

    Klass* k = x->klass();
    while (k != nullptr) {
        if (k == ((HiTypeObject*)y)->own_klass()) {
            return Universe::HiTrue;
        }

        k = k->super();
    }

    return Universe::HiFalse;
}

HiObject* type_of(ArrayList<HiObject*>* args) {
    HiObject* arg0 = args->get(0);

    if (arg0->klass() == NativeFunctionKlass::get_instance()) {
        arg0 = TypeKlass::get_instance()->type_object();
    }

    return arg0->klass()->type_object();
}

HiObject* FunctionObject::call(ArrayList<HiObject*>* args) {
    return _native_func(args);
}

/*
 * MethodObject.
 */

MethodKlass* MethodKlass::instance = nullptr;

MethodKlass* MethodKlass::get_instance() {
    if (instance == nullptr) {
        instance = new MethodKlass();
    }

    return instance;
}

MethodKlass::MethodKlass() {
    set_klass_dict(new HiDict());
}

bool MethodObject::is_function(HiObject* x) {
    Klass* k = x->klass();
    if (k == (Klass*)FunctionKlass::get_instance()) {
        return true;
    }

    while (k->super() != nullptr) {
        k = k->super();
        if (k == (Klass*)FunctionKlass::get_instance()) {
            return true;
        }
    }

    return false;
}
