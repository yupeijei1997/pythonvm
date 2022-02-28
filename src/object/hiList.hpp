#ifndef HI_LIST_HPP
#define HI_LIST_HPP

#include "object/klass.hpp"
#include "object/hiObject.hpp"
#include "util/arrayList.hpp"

class ListKlass : public Klass {
private:
    ListKlass() {}
    static ListKlass* instance;

public:
    static ListKlass* get_instance();

    virtual void print(HiObject* obj);
    
    virtual HiObject* subscr(HiObject* x, HiObject* y);
};

class HiList : public HiObject {
friend class ListKlass;

private:
    ArrayList<HiObject*>* _inner_list;

public:
    HiList();
    HiList(ArrayList<HiObject*>* ol);

    ArrayList<HiObject*>* inner_list() { return _inner_list; }

    int size() { return _inner_list->size(); }
    void append(HiObject* obj) { _inner_list->add(obj); }
    HiObject* pop(HiObject* obj) { return _inner_list->pop(); }
    HiObject* get(int index) { return _inner_list->get(index); }
    void set(int i, HiObject* o) { _inner_list->set(i, o); }
    HiObject* top() { get(size() - 1); }
};

#endif