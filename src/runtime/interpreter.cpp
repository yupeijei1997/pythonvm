#include <stdio.h>
#include <typeinfo>
#include <iostream>
#include "runtime/interpreter.hpp"
#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"
#include "runtime/stringTable.hpp"
#include "runtime/cellObject.hpp"
#include "code/codeObject.hpp"
#include "code/bytecode.hpp"
#include "util/map.hpp"
#include "object/hiList.hpp"
#include "object/hiDict.hpp"

#define PUSH(x) _frame->stack()->add(x)
#define POP() _frame->stack()->pop()
#define TOP() _frame->stack()->top()
#define STACK_LEVEL() _frame->stack()->size()

#define HI_TRUE Universe::HiTrue
#define HI_FALSE Universe::HiFalse

using namespace std;

Interpreter::Interpreter() {
    _builtins = new Map<HiObject*, HiObject*>();
    _builtins->put(new HiString("True"), Universe::HiTrue);
    _builtins->put(new HiString("False"), Universe::HiFalse);
    _builtins->put(new HiString("None"), Universe::HiNone);
    _builtins->put(new HiString("len"), new FunctionObject(len));
    _builtins->put(new HiString("isinstance"), new FunctionObject(isinstance));
    _builtins->put(new HiString("type"), new FunctionObject(type_of));

    _builtins->put(new HiString("object"), ObjectKlass::get_instance()->type_object());
    _builtins->put(new HiString("int"), IntegerKlass::get_instance()->type_object());
    _builtins->put(new HiString("str"), StringKlass::get_instance()->type_object());
    _builtins->put(new HiString("list"), ListKlass::get_instance()->type_object());
    _builtins->put(new HiString("dict"), DictKlass::get_instance()->type_object());
}

void Interpreter::build_frame(HiObject* callable, ArrayList<HiObject*>* args, int op_arg) {
    if (callable->klass() == NativeFunctionKlass::get_instance()) {
        PUSH(((FunctionObject*)callable)->call(args));
    }
    else if (callable->klass() == MethodKlass::get_instance()) {
        MethodObject* method = (MethodObject*)callable;
        if (!args) {
            args = new ArrayList<HiObject*>(1);
        }
        args->insert(0, method->owner());
        build_frame(method->func(), args, op_arg + 1);
    }
    else if (callable->klass() == FunctionKlass::get_instance()) {
        FrameObject* frame = new FrameObject((FunctionObject*)callable, args, op_arg);
        frame->set_sender(_frame);
        _frame = frame;
    }
    else if (callable->klass() == TypeKlass::get_instance()) {
        HiObject* inst = ((HiTypeObject*)callable)->own_klass()->allocate_instance(callable, args);
        PUSH(inst);
    }
}

void Interpreter::destory_frame() {
    FrameObject* temp = _frame;
    _frame = _frame->sender();
    delete temp;
}

void Interpreter::leave_frame() {
    destory_frame();
    PUSH(_ret_value);
}

void Interpreter::run(CodeObject* codes) {
    _frame = new FrameObject(codes);
    eval_frame();
    destory_frame();
}

void Interpreter::eval_frame() {

    while (_frame->has_more_codes()) {
        unsigned char op_code = _frame->get_op_code();
        bool has_argument = op_code >= ByteCode::HAVE_ARGUMENT;

        int op_arg = -1;
        if (has_argument) {
            op_arg = _frame->get_op_arg();
        }

        HiObject* u, * v, * w;
        Block* b;
        FunctionObject* fo;
        ArrayList<HiObject*>* args = nullptr;
        switch (op_code) {
        case ByteCode::POP_TOP:
            POP();
            break;

        case ByteCode::LOAD_CONST:
            v = _frame->_consts->get(op_arg);
            PUSH(v);
            break;

        case ByteCode::LOAD_NAME:
            v = _frame->names()->get(op_arg);
            w = _frame->locals()->get(v);
            if (w != Universe::HiNone) {
                PUSH(w);
                break;
            }

            w = _frame->globals()->get(v);
            if (w != Universe::HiNone) {
                PUSH(w);
                break;
            }

            w = _builtins->get(v);
            if (w != Universe::HiNone) {
                PUSH(w);
                break;
            }

            PUSH(Universe::HiNone);
            break;

        case ByteCode::LOAD_GLOBAL:
            v = _frame->names()->get(op_arg);
            w = _frame->globals()->get(v);
            if (w != Universe::HiNone) {
                PUSH(w);
                break;
            }
            
            w = _builtins->get(v);
            if (w != Universe::HiNone) {
                PUSH(w);
                break;
            }

            PUSH(Universe::HiNone);
            break;

        case ByteCode::LOAD_FAST:
            PUSH(_frame->fast_locals()->get(op_arg));
            break;

        case ByteCode::LOAD_ATTR:
            v = POP();
            w = _frame->names()->get(op_arg);
            PUSH(v->getattr(w));
            break;

        case ByteCode::LOAD_CLOSURE:
            v = _frame->closure()->get(op_arg);  // 从局部变量(cellvars)和 freevars 中获取
            if (v == nullptr) {
                v = _frame->get_cell_from_parameter(op_arg);  // 从入参(cellvars)中获取
                _frame->closure()->set(op_arg, v);
            }

            if (v->klass() == CellKlass::get_instance()) {
                // v 来自 freevars 中
                PUSH(v);
            }
            else {
                PUSH(new CellObject(_frame->closure(), op_arg));
            }
            break;

        case ByteCode::LOAD_DEREF:
            v = _frame->closure()->get(op_arg);
            if (v == nullptr) {
                w = _frame->codes()->_cell_vars->get(op_arg);
                int index = _frame->codes()->_var_names->index(w);
                v = _frame->fast_locals()->get(index);  // v 来自入参(cellvars)中
            }
            else {
                if (v->klass() == CellKlass::get_instance()) {
                    v = ((CellObject*)v)->value();  // v 来自 freevars 中
                }
            }
            PUSH(v);
            break;

        case ByteCode::LOAD_LOCALS:
            PUSH(_frame->locals());
            break;

        case ByteCode::STORE_NAME:
            v = _frame->names()->get(op_arg);
            _frame->locals()->put(v, POP());
            break;

        case ByteCode::STORE_GLOBAL:
            v = _frame->names()->get(op_arg);
            _frame->globals()->put(v, POP());
            break;

        case ByteCode::STORE_FAST:
            _frame->fast_locals()->set(op_arg, POP());
            break;

        case ByteCode::STORE_SUBSCR:
            u = POP();
            v = POP();
            w = POP();
            v->store_subscr(u, w);
            break;

        case ByteCode::STORE_MAP:
            u = POP();
            v = POP();
            w = TOP();
            ((HiDict*)w)->put(u, v);
            break;

        case ByteCode::STORE_DEREF:
            _frame->closure()->set(op_arg, POP());
            break;

        case ByteCode::DELETE_SUBSCR:
            v = POP();
            w = POP();
            w->del_subscr(v);
            break;

        case ByteCode::PRINT_ITEM:
            v = POP();
            v->print();
            break;
        
        case ByteCode::PRINT_NEWLINE:
            printf("\n");
            break;

        case ByteCode::BINARY_SUBSCR:
            v = POP();
            w = POP();
            PUSH(w->subscr(v));
            break;

        case ByteCode::BINARY_ADD:
            v = POP();
            w = POP();
            PUSH(w->add(v));
            break;

        case ByteCode::BINARY_MULTIPLY:
            v = POP();
            w = POP();
            PUSH(v->mul(w));
            break;

        case ByteCode::RETURN_VALUE:
            _ret_value = POP();
            if (_frame->is_first_frame()) {
                return;
            }
            leave_frame();
            break;

        case ByteCode::COMPARE_OP:
            v = POP();
            w = POP();

            switch (op_arg) {
            case ByteCode::LESS:
                PUSH(w->less(v));
                break;

            case ByteCode::GREATER:
                PUSH(w->greater(v));
                break;
            
            case ByteCode::EQUAL:
                PUSH(w->equal(v));
                break;

            case ByteCode::NOT_EQUAL:
                PUSH(w->not_equal(v));
                break;

            case ByteCode::LESS_EQUAL:
                PUSH(w->le(v));
                break;

            case ByteCode::GREATER_EQUAL:
                PUSH(w->ge(v));
                break;

            case ByteCode::IN:
                PUSH(v->contains(w));
                break;

            case ByteCode::NOT_IN:
                PUSH(v->not_contains(w));
                break;

            case ByteCode::IS:
                if (w == v) {
                    PUSH(HI_TRUE);
                }
                else {
                    PUSH(HI_FALSE);
                }
                break;

            case ByteCode::IS_NOT:
                if (w != v) {
                    PUSH(HI_TRUE);
                }
                else {
                    PUSH(HI_FALSE);
                }
                break;

            default:
                printf("Error: unrecognized compare op %d", op_arg);
            }
            break;

        case ByteCode::POP_JUMP_IF_FALSE:
            v = POP();
            if (v == Universe::HiFalse) {
                _frame->set_pc(op_arg);
            }
            break;

        case ByteCode::JUMP_FORWARD:
            _frame->set_pc(_frame->get_pc() + op_arg);
            break;

        case ByteCode::JUMP_ABSOLUTE:
            _frame->set_pc(op_arg);
            break;

        case ByteCode::SETUP_LOOP:
            _frame->loop_stack()->add(new Block(op_code, _frame->get_pc() + op_arg, STACK_LEVEL()));
            break;

        case ByteCode::POP_BLOCK:
            b = _frame->loop_stack()->pop();
            if (STACK_LEVEL() > b->_level) {
                POP();
            }
            break;

        case ByteCode::BREAK_LOOP:
            b = _frame->loop_stack()->pop();
            if (STACK_LEVEL() > b->_level) {
                POP();
            }
            _frame->set_pc(b->_target);
            break;

        case ByteCode::MAKE_FUNCTION:
            v = POP();
            fo = new FunctionObject(v);
            fo->set_globals(_frame->globals());
            if (op_arg > 0) {
                args = new ArrayList<HiObject*>(op_arg);
                while (op_arg--) {
                    args->set(op_arg, POP());
                }
            }
            fo->set_defaults(args);
            if (args != nullptr) {
                delete args;
                args = nullptr;
            }
            
            PUSH(fo);
            break;

        case ByteCode::MAKE_CLOSURE:
            v = POP();
            fo = new FunctionObject(v);
            fo->set_closure((HiList*)(POP()));
            fo->set_globals(_frame->globals());
            if (op_arg > 0) {
                args = new ArrayList<HiObject*>(op_arg);
                while (op_arg--) {
                    args->set(op_arg, POP());
                }
            }
            fo->set_defaults(args);

            if (args != nullptr) {
                delete args;
                args = nullptr;
            }

            PUSH(fo);
            break;

        case ByteCode::CALL_FUNCTION:
            if (op_arg > 0) {
                int na = op_arg & 0xFF;
                int nk = op_arg >> 8;
                int arg_cnt = na + nk * 2;
                args = new ArrayList<HiObject*>(arg_cnt);  // 实参
                while (arg_cnt--) {
                    args->set(arg_cnt, POP());
                }
            }
            build_frame(POP(), args, op_arg);
            if (args != nullptr) {
                delete args;
                args = nullptr;
            }
            break;

        case ByteCode::BUILD_TUPLE:
            v = new HiList();
            while (op_arg--) {
                ((HiList*)v)->set(op_arg, POP());
            }
            PUSH(v);
            break;

        case ByteCode::BUILD_LIST:
            v = new HiList();
            while (op_arg--) {
                ((HiList*)v)->set(op_arg, POP());
            }
            PUSH(v);
            break;

        case ByteCode::BUILD_MAP:
            v = new HiDict();
            PUSH(v);
            break;

        case ByteCode::BUILD_CLASS:
            u = POP();
            v = POP();
            w = POP();
            v = Klass::create_klass(u, v, w);
            PUSH(v);
            break;

        case ByteCode::GET_ITER:
            v = POP();
            PUSH(v->iter());
            break;

        case ByteCode::FOR_ITER:
            v = TOP();
            w = v->next();
            
           if (w == nullptr) {
                _frame->set_pc(_frame->_pc + op_arg);
                POP();
            }
            else {
                PUSH(w);
            }
            break;

        case ByteCode::UNPACK_SEQUENCE:
            v = POP();

            while (op_arg--) {
                PUSH(v->unpack(new HiInteger(op_arg)));
            }
            break;

        default:
            printf("Error: Unrecognized byte code %d\n", op_code);
        }
    }
}
