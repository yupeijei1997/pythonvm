#include "code/codeObject.hpp"

CodeObject::CodeObject(int argcount, int nlocals, int stacksize, int flag, HiString* bytecodes,
    ArrayList<HiObject*>* consts, ArrayList<HiObject*>* names,
    ArrayList<HiObject*>* varnames,
    ArrayList<HiObject*>* freevars, ArrayList<HiObject*>* cellvars,
    HiString* coname, HiString* cofilename, int lineno, HiString* notable) :
        _argcount(argcount),
        _nlocals(nlocals),
        _stack_size(stacksize),
        _flag(flag),
        _bytecodes(bytecodes),
        _consts(consts),
        _names(names),
        _var_names(varnames),
        _free_vars(freevars),
        _cell_vars(cellvars),
        _co_name(coname),
        _co_file_name(cofilename),
        _lineno(lineno),
        _notable(notable) {
}
