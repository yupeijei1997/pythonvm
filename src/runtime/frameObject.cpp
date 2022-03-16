#include "object/hiList.hpp"
#include "runtime/frameObject.hpp"


FrameObject::FrameObject(CodeObject* codes) {
    _consts = codes->_consts;
    _names = codes->_names;

    _locals = new HiDict();
    _globals = _locals;
    _locals->put(new HiString("__name__"), new HiString("__main__"));
    _stack = new ArrayList<HiObject*>();
    _loop_stack = new ArrayList<Block*>();

    _codes = codes;
    _pc = 0;
    _sender = nullptr;
    _entry_frame = false;
}

FrameObject::FrameObject(FunctionObject* func, ArrayList<HiObject*>* args, int op_arg) {
    _codes = func->_func_code;
    _consts = _codes->_consts;
    _names = _codes->_names;

    _locals = new HiDict();
    _globals = func->globals();
    _fast_locals = new ArrayList<HiObject*>();
    const int argcnt = _codes->_argcount;
    const int na = op_arg & 0xFF;
    const int nk = op_arg >> 8;
    int kw_pos = argcnt;
    
    if (func->defaults()) {
        int dft_num = func->defaults()->length();
        int arg_num = _codes->_argcount;

        while (dft_num--) {
            _fast_locals->set(--arg_num, func->defaults()->get(dft_num));
        }
    }

    HiList* alist = new HiList();
    HiDict* adict = new HiDict();

    // 处理位置参数和扩展位置参数
    if (argcnt < na) {
        int i = 0;
        for (; i < argcnt; ++i) {
            _fast_locals->set(i, args->get(i));
        }

        for (; i < na; ++i) {
            alist->append(args->get(i));
        }
    }
    else {
        for (int i = 0; i < na; ++i) {
            _fast_locals->set(i, args->get(i));
        }
    }

    // 处理键参数和扩展键参数
    for (int i = 0; i < nk; ++i) {
        HiObject* key = args->get(na + i * 2);
        HiObject* value = args->get(na + i * 2 + 1);

        int index = _codes->_var_names->index(key);
        if (index > 0) {
            _fast_locals->set(index, value);
        }
        else {
            adict->put(key, value);
        }
    }

    if (_codes->_flag & FunctionObject::CO_VARARGS) {
        _fast_locals->set(argcnt, alist);
        ++kw_pos;
    }
    
    if (_codes->_flag & FunctionObject::CO_VARKEYWORDS) {
        _fast_locals->set(kw_pos, adict);
    }

    _closure = nullptr;
    ArrayList<HiObject*>* cells = _codes->_cell_vars;
    if (cells && cells->size() > 0) {
        _closure = new HiList();

        for (int i = 0; i < cells->size(); ++i) {
            _closure->append(nullptr);
        }
    }

    if (func->closure() && func->closure()->size() > 0) {
        if (_closure == nullptr) {
            _closure = func->closure();
        }
        else {
            _closure = (HiList*)(_closure->add(func->closure()));
        }
    }

    _stack = new ArrayList<HiObject*>();
    _loop_stack = new ArrayList<Block*>();

    _pc = 0;
    _sender = nullptr;
    _entry_frame = false;
}

int FrameObject::get_op_arg() {
    int byte1 = _codes->_bytecodes->value()[_pc++] & 0xFF;
    int byte2 = _codes->_bytecodes->value()[_pc++];
    return byte2 << 8 | byte1;
}

unsigned char FrameObject::get_op_code() {
    return _codes->_bytecodes->value()[_pc++];
}

bool FrameObject::has_more_codes() {
    return _pc < _codes->_bytecodes->length();
}

HiObject* FrameObject::get_cell_from_parameter(int i) {
    HiObject* cell_name = _codes->_cell_vars->get(i);
    i = _codes->_var_names->index(cell_name);
    return _fast_locals->get(i);
}
