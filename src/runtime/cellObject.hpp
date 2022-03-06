#ifndef CELL_OBJECT_HPP
#define CELL_OBJECT_HPP

#include "object/klass.hpp"
#include "object/hiObject.hpp"
#include "object/hiList.hpp"

class CellKlass : public Klass{
private:
    CellKlass();
    static CellKlass* instance;

public:
    static CellKlass* get_instance();
};

class CellObject : public HiObject {
friend class CellKlass;

private:
    HiList* _table;
    int _index;

public:
    CellObject(HiList* l, int i);
    HiObject* value();
};

#endif