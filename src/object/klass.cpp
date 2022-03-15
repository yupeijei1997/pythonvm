#include "object/klass.hpp"
#include "object/hiInteger.hpp"
#include "object/hiList.hpp"
#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"

int Klass::compare_klass(Klass* x, Klass* y) {
    if (x == y) {
        return 0;
    }

    if (x == IntegerKlass::get_instance()) {
        return -1;
    }
    else if (y == IntegerKlass::get_instance()) {
        return 1;
    }

    if (x->name()->less(y->name()) == Universe::HiTrue) {
        return -1;
    }
    else {
        return 1;
    }
}

HiObject* Klass::create_klass(HiObject* x, HiObject* supers, HiObject* name) {
    assert(x && x->klass() == DictKlass::get_instance());
    assert(supers && supers->klass() == ListKlass::get_instance());
    assert(name && name->klass() == StringKlass::get_instance());

    Klass* new_klass = new Klass();
    HiDict* klass_dict = (HiDict*)x;
    HiList* supers_list = (HiList*)supers;
    HiString* name_str = (HiString*)name;
    new_klass->set_klass_dict(klass_dict);
    new_klass->set_super_list(supers_list);
    new_klass->set_name(name_str);

    HiTypeObject* type_obj = new HiTypeObject();
    type_obj->set_own_klass(new_klass);

    return type_obj;
}

void Klass::add_super(Klass* klass) {
    if (_super == nullptr) {
        _super = new HiList();
    }

    _super->append(klass->type_object());
}

HiTypeObject* Klass::super() {
    if (_super == nullptr || _super->size() <= 0) {
        return nullptr;
    }

    return (HiTypeObject*)(_super->get(0));
}

HiObject* Klass::allocate_instance(HiObject* callable, ArrayList<HiObject*>* args) {
    HiObject* inst = new HiObject();
    Klass* k = ((HiTypeObject*)callable)->own_klass();
    inst->set_klass(k);
    return inst;
}

HiObject* Klass::getattr(HiObject* x, HiObject* y) {
    HiObject* result = Universe::HiNone;

    if (x->obj_dict() != nullptr) {
        result = x->obj_dict()->get(y);
        if (result != Universe::HiNone) {
            return result;
        }
    }

    result = x->klass()->klass_dict()->get(y);
    if (result == Universe::HiNone) {
        return result;
    }

    if (MethodObject::is_function(result)) {
        result = new MethodObject(x, (FunctionObject*)result);
    }
    return result;
}

HiObject* Klass::setattr(HiObject* x, HiObject* y, HiObject* z) {
    if (x->obj_dict() == nullptr) {
        x->init_dict();
    }

    x->obj_dict()->put(y, z);
    return Universe::HiNone;
}
