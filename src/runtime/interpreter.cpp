#include <stdio.h>
#include <typeinfo>
#include <iostream>
#include "runtime/interpreter.hpp"
#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"
#include "code/codeObject.hpp"
#include "code/bytecode.hpp"
#include "util/map.hpp"
#include "object/hiList.hpp"

#define PUSH(x) _frame->stack()->add(x)
#define POP() _frame->stack()->pop()
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
}

void Interpreter::build_frame(HiObject* callable, ArrayList<HiObject*>* args) {
    if (callable->klass() == NativeFunctionKlass::get_instance()) {
        PUSH(((FunctionObject*)callable)->call(args));
    }
    else if (callable->klass() == MethodKlass::get_instance()) {
        MethodObject* method = (MethodObject*)callable;
        if (!args) {
            args = new ArrayList<HiObject*>(1);
        }
        args->insert(0, method->owner());
        build_frame(method->func(), args);
    }
    else if (callable->klass() == FunctionKlass::get_instance()) {
        FrameObject* frame = new FrameObject((FunctionObject*)callable, args);
        frame->set_sender(_frame);
        _frame = frame;
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

        case ByteCode::CALL_FUNCTION:
            if (op_arg > 0) {
                args = new ArrayList<HiObject*>(op_arg);
                while (op_arg--) {
                    args->set(op_arg, POP());
                }
            }
            build_frame(POP(), args);
            if (args != nullptr) {
                delete args;
                args = nullptr;
            }
            break;

        case ByteCode::BUILD_LIST:
            v = new HiList();
            while (op_arg--) {
                ((HiList*)v)->set(op_arg, POP());
            }
            PUSH(v);
            break;

        default:
            printf("Error: Unrecognized byte code %d\n", op_code);
        }
    }
}
