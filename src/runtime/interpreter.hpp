#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "util/arrayList.hpp"
#include "code/codeObject.hpp"
#include "runtime/frameObject.hpp"
#include "util/map.hpp"

class Interpreter {
private:
    Map<HiObject*, HiObject*>* _builtins;
    FrameObject* _frame;
    HiObject* _ret_value;

public:
    Interpreter();

    void run(CodeObject* codes);
    void eval_frame();
    void destory_frame();

    void build_frame(HiObject* callable, ArrayList<HiObject*>* args, int op_arg);
    void leave_frame();
};

#endif