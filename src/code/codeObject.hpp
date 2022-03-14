#ifndef CODE_OBJECT_HPP
#define CODE_OBJECT_HPP

#include "object/hiObject.hpp"
#include "object/hiString.hpp"
#include "object/hiInteger.hpp"
#include "util/arrayList.hpp"

class CodeObject : public HiObject {
public:
    int _argcount;
    int _nlocals;
    int _stack_size;
    int _flag;

    HiString* _bytecodes;
    ArrayList<HiObject*>* _consts;
    ArrayList<HiObject*>* _names;
    ArrayList<HiObject*>* _var_names;

    ArrayList<HiObject*>* _free_vars;
    ArrayList<HiObject*>* _cell_vars;

    HiString* _co_file_name;
    HiString* _co_name;
    
    int _lineno;
    HiString* _notable;

    CodeObject(int argcount, int nlocals, int stacksize, int flag, HiString* bytecodes,
        ArrayList<HiObject*>* consts, ArrayList<HiObject*>* names,
        ArrayList<HiObject*>* varnames,
        ArrayList<HiObject*>* cellvars, ArrayList<HiObject*>* freevars,
        HiString* cofilename, HiString* coname,
        int lineno, HiString* notable);
};

#endif