#ifndef FRAME_OBJECT_HPP
#define FRAME_OBJECT_HPP

#include "object/hiObject.hpp"
#include "object/hiDict.hpp"
#include "util/arrayList.hpp"
#include "util/map.hpp"
#include "code/codeObject.hpp"
#include "runtime/functionObject.hpp"

class Block {
public:
    unsigned char _type;
    unsigned int _target;
    int _level;

public:
    Block() : _type(0), _target(0), _level(0) {}
    
    Block(unsigned char b_type, unsigned int b_target, int b_level) :
        _type(b_type), _target(b_target), _level(b_level) {}

    Block(const Block& b) {
        _type = b._type;
        _target = b._target;
        _level = b._level;
    }
};

class FrameObject {
public:
    ArrayList<HiObject*>* _stack;
    ArrayList<Block*>* _loop_stack;

    ArrayList<HiObject*>* _consts;
    ArrayList<HiObject*>* _names;

    HiDict* _locals;
    HiDict* _globals;
    ArrayList<HiObject*>* _fast_locals;

    CodeObject* _codes;

    int _pc;

    FrameObject* _sender;

public:
    FrameObject(CodeObject* codes);
    FrameObject(FunctionObject* func, ArrayList<HiObject*>* args, int op_arg);
    ~FrameObject() {}

    void set_pc(int x) { _pc = x; }
    int get_pc() { return _pc; }

    void set_sender(FrameObject* x) { _sender = x; }
    FrameObject* sender() { return _sender; }

    ArrayList<HiObject*>* stack() { return _stack; }
    ArrayList<Block*>* loop_stack() { return _loop_stack; }
    ArrayList<HiObject*>* consts() { return _consts; }
    ArrayList<HiObject*>* names() { return _names; }
    HiDict* locals() { return _locals; }
    HiDict* globals() { return _globals; }
    ArrayList<HiObject*>* fast_locals() { return _fast_locals; }

    bool is_first_frame() { return _sender == nullptr; }
    bool has_more_codes();
    unsigned char get_op_code();
    int get_op_arg();
};

#endif