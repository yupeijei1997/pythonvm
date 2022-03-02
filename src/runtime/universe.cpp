#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"
#include "object/hiString.hpp"
#include "util/map.hpp"

HiInteger* Universe::HiTrue = nullptr;
HiInteger* Universe::HiFalse = nullptr;

HiObject* Universe::HiNone = nullptr;

void Universe::genesis() {
    HiTrue = new HiInteger(1);
    HiFalse = new HiInteger(0);
    HiNone = new HiObject();

    Map<HiObject*, HiObject*>* klass_dict = new Map<HiObject*, HiObject*>();
    klass_dict->put(new HiString("upper"), new FunctionObject(string_upper));
    StringKlass::get_instance()->set_klass_dict(klass_dict);
    StringKlass::get_instance()->set_name(new HiString("str"));
}

void Universe::destory() {
}
