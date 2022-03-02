#ifndef HI_LIST_HPP
#define HI_LIST_HPP

#include "object/klass.hpp"
#include "object/hiObject.hpp"
#include "util/arrayList.hpp"

class ListKlass : public Klass {
private:
    ListKlass();
    static ListKlass* instance;

public:
    static ListKlass* get_instance();

    virtual void print(HiObject* obj);
    
    virtual HiObject* less(HiObject* x, HiObject* y);
    virtual HiObject* subscr(HiObject* x, HiObject* y);
    virtual HiObject* contains(HiObject* x, HiObject* y);
    virtual HiObject* not_contains(HiObject* x, HiObject* y);
    virtual void store_subscr(HiObject* x, HiObject* y, HiObject* z);
    virtual void del_subscr(HiObject* x, HiObject* y);
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
    void insert(int i, HiObject* o) { _inner_list->insert(i, o); }
    HiObject* pop() { return _inner_list->pop(); }
    HiObject* get(int index) { return _inner_list->get(index); }
    void set(int i, HiObject* o) { _inner_list->set(i, o); }
    HiObject* top() { return get(size() - 1); }
    void delete_index(int index) { return _inner_list->delete_index(index); }
};

HiObject* list_append(ArrayList<HiObject*>* args);
HiObject* list_insert(ArrayList<HiObject*>* args);
HiObject* list_index(ArrayList<HiObject*>* args);
HiObject* list_pop(ArrayList<HiObject*>* args);
HiObject* list_remove(ArrayList<HiObject*>* args);
HiObject* list_reverse(ArrayList<HiObject*>* args);
HiObject* list_sort(ArrayList<HiObject*>* args);

#endif