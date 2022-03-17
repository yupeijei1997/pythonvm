#include <stdio.h>
#include "object/klass.hpp"
#include "object/hiInteger.hpp"
#include "object/hiList.hpp"
#include "runtime/universe.hpp"
#include "runtime/stringTable.hpp"
#include "runtime/functionObject.hpp"
#include "runtime/interpreter.hpp"

#define ST(x) StringTable::get_instance()->STR(x)
#define STR(x) x##_str

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

void Klass::print(HiObject* x) {
    printf("<");
    HiDict* attr_dict = x->klass()->klass_dict();
    if (attr_dict) {
        HiObject* mod = attr_dict->get(StringTable::get_instance()->mod_str);
        if (mod != Universe::HiNone) {
            mod->print();
            printf(".");
        }
    }
    x->klass()->name()->print();
    printf(" object at ");
    printf("%p>", x);
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

HiObject* Klass::greater(HiObject* x, HiObject* y) {
    ArrayList<HiObject*>* args = new ArrayList<HiObject*>();
    args->add(y);
    return find_and_call(x, args, ST(gt));
}

HiObject* Klass::less(HiObject* x, HiObject* y) {
    ArrayList<HiObject*>* args = new ArrayList<HiObject*>();
    args->add(y);
    return find_and_call(x, args, ST(lt));
}

HiObject* Klass::equal(HiObject* x, HiObject* y) {
    ArrayList<HiObject*>* args = new ArrayList<HiObject*>();
    args->add(y);
    return find_and_call(x, args, ST(eq));
}

HiObject* Klass::not_equal(HiObject* x, HiObject* y) {
    ArrayList<HiObject*>* args = new ArrayList<HiObject*>();
    args->add(y);
    return find_and_call(x, args, ST(ne));
}

HiObject* Klass::ge(HiObject* x, HiObject* y) {
    ArrayList<HiObject*>* args = new ArrayList<HiObject*>();
    args->add(y);
    return find_and_call(x, args, ST(ge));
}

HiObject* Klass::le(HiObject* x, HiObject* y) {
    ArrayList<HiObject*>* args = new ArrayList<HiObject*>();
    args->add(y);
    return find_and_call(x, args, ST(le));
}

HiObject* Klass::add(HiObject* x, HiObject* y) {
    ArrayList<HiObject*>* args = new ArrayList<HiObject*>();
    args->add(y);
    return find_and_call(x, args, ST(add));
}

HiObject* Klass::sub(HiObject* x, HiObject* y) {
    ArrayList<HiObject*>* args = new ArrayList<HiObject*>();
    args->add(y);
    return find_and_call(x, args, ST(sub));
}

HiObject* Klass::mul(HiObject* x, HiObject* y) {
    ArrayList<HiObject*>* args = new ArrayList<HiObject*>();
    args->add(y);
    return find_and_call(x, args, ST(mul));
}

HiObject* Klass::div(HiObject* x, HiObject* y) {
    ArrayList<HiObject*>* args = new ArrayList<HiObject*>();
    args->add(y);
    return find_and_call(x, args, ST(div));
}

HiObject* Klass::mod(HiObject* x, HiObject* y) {
    ArrayList<HiObject*>* args = new ArrayList<HiObject*>();
    args->add(y);
    return find_and_call(x, args, ST(modulo));
}

HiObject* Klass::find_and_call(HiObject* x, ArrayList<HiObject*>* args, HiObject* func_name) {
    HiObject* func = x->getattr(func_name);
    if (func != Universe::HiNone) {
        return Interpreter::get_instance()->call_virtual(func, args);
    }

    printf("class");
    x->klass()->name()->print();
    printf("Error : unsupport operation for class ");
    assert(false);
}

HiObject* Klass::allocate_instance(HiObject* callable, ArrayList<HiObject*>* args) {
    HiObject* inst = new HiObject();
    Klass* k = ((HiTypeObject*)callable)->own_klass();
    inst->set_klass(k);

    HiObject* constructor = inst->getattr(StringTable::get_instance()->init_str);

    if (constructor != Universe::HiNone) {
        HiObject* result = Interpreter::get_instance()->call_virtual(constructor, args);
    }
    return inst;
}

HiObject* Klass::len(HiObject* x) {
    return find_and_call(x, nullptr, ST(len));
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
