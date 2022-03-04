#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"
#include "object/hiString.hpp"
#include "object/hiDict.hpp"
#include "object/hiList.hpp"
#include "util/map.hpp"

HiInteger* Universe::HiTrue = nullptr;
HiInteger* Universe::HiFalse = nullptr;

HiObject* Universe::HiNone = nullptr;

void Universe::genesis() {
    HiTrue = new HiInteger(1);
    HiFalse = new HiInteger(0);
    HiNone = new HiObject();

    DictKlass::get_instance()->initialize();
    StringKlass::get_instance()->initialize();
    IntegerKlass::get_instance()->initialize();
    ListKlass::get_instance()->initialize();
}

void Universe::destory() {
}
