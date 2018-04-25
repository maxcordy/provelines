/*
 * LList.hpp
 *
 * Created on 2013-07-09
 */

#ifndef UTIL_LLIST_HPP
#define UTIL_LLIST_HPP

#include "Assert.hpp"
#include "List.hpp"
#include "Uncopyable.hpp"

#include <iostream>
#include <string>

using std::auto_ptr;
using std::cout;
using std::endl;
using std::string;

namespace util {

/**
 * LLists, aka labeled lists, are mutable lists of elements mapping a label
 * to a value of type T.
 *
 * Defensive copying is performed on the elements that are inserted. In case
 * of big objects, pointers can be stored instead. In this case the list
 * elements can be modified from the outside of the list.
 *
 * Testing for equality is done using the == operator. As such this operator
 * must be implemented by llist values.
 *
 * Labeled lists cannot be copied, that is invoking the copy constructor
 * or the copy assignment operator is forbidden.
 *
 * @author mwi
 */
template <typename T>
class LList : private Uncopyable {

private:
    struct Node {
        const std::string label;
        const T value;
        Node * next;
        Node * prev;

        Node(Node * prev, const std::string & label, const T & value,
                Node * next)
            : label(label), value(value), next(next), prev(prev) {}
    };

    /** The size of the list. */
    int size;
    /** Pointer to first node. */
    Node * first;
    /** Pointer to last node. */
    Node * last;

    /*
     * Representation Invariant:
     *   I(c) = (c.first = null && c.last = null && c.size = 0) ||
     *          ( c.first != null && c.last != null && c.size > 0 &&
     *            c.first.prev = null && c.last.next = null
     *          )
     */

public:
    /**
     * @effects Makes this be a new empty labeled list.
     */
    LList();

    /**
     * @effects Destroys this.
     */
    ~LList();

    /**
     * @modifies this
     * @effects Adds the element (label,value) to this.
     */
    void add(const std::string & label, const T & value);

    /**
     * @modifies this
     * @effects Removes the first element e with e.label = 'label'.
     * @return true iff 'label' is in this.
     */
    bool removeLabel(const std::string & label);

    /**
     * @modifies this
     * @effects Removes the first element e with e.value = 'value' in this.
     * @return true iff 'value' is in this.
     */
    bool remove(const T & value);

    /**
     * @modifies this
     * @effects Merges this with 'llist'. If the lists share a label, only
     *           that of this is kept.
     */
    void merge(const LList<T> & llist);

    /**
     * @modifies this
     * @effects Concanates the labeled list 'llist' with this.
     */
    void concat(const LList<T> & llist);

    /**
     * @requires there exists an element e with e.label = 'label' in this
     * @return the first element in this with label 'label'.
     */
    const T & find(const std::string & label) const;

    /**
     * @return true iff there exists an element e with e.label = 'label'
     *          in this.
     */
    bool contains(const std::string & label) const;

    /**
     * @return the number of elements in this.
     */
    int count() const;

    /**
     * @modifies stdout
     * @effects Prints this on stdout.
     */
    void print() const;

    /**
     * @return a new labeled list 'llist' from the list 'list' such that
     *          for all 0 <= i < list.count .(
     *           llist = [(list[i].element; i)]
     *         )
     */
    static std::auto_ptr< LList<int> > list2llist(const List<std::string> &
            list);

private:
    /**
     * @requires x != null
     * @modifies this, x
     * @effects Unlinks node x and destroys x.
     */
    void unlink(Node * x);

    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;
};

template <typename T>
LList<T>::LList()
    : size(0), first(NULL), last(NULL) {
    checkRep();
}

template <typename T>
LList<T>::~LList() {
    while (first != NULL) {
        Node * tmp = first;
        first = first->next;
        delete tmp;
    }
}

template <typename T>
void LList<T>::add(const string & label, const T & value) {
    Node * const l = last;
    Node * const newNode = new Node(l, label, value, NULL);
    last = newNode;
    if (l == NULL) {
        first = newNode;
    } else {
        l->next = newNode;
    }
    size++;

    checkRep();
}

template <typename T>
bool LList<T>::removeLabel(const string & label) {
    bool found = false;
    for (Node * x = first; x != NULL; x = x->next) {
        if (label == x->label) {
            unlink(x);
            found = true;
            break;
        }
    }
    checkRep();
    return found;
}

template <typename T>
bool LList<T>::remove(const T & value) {
    bool found = false;
    for (Node * x = first; x != NULL; x = x->next) {
        if (value == x->value) {
            unlink(x);
            found = true;
            break;
        }
    }
    checkRep();
    return found;
}

template <typename T>
void LList<T>::merge(const LList & llist) {
    for (Node * x = llist.first; x != NULL; x = x -> next) {
        if (!contains(x->label)) {
            add(x->label, x->value);
        }
    }
}

template <typename T>
void LList<T>::concat(const LList<T> & llist) {
    int expectedSize = size + llist.size;

    for (Node * x = llist.first; x != NULL; x = x ->next) {
        add(x->label, x->value);
    }

    assertTrue(size == expectedSize);
    checkRep();
}

template <typename T>
const T & LList<T>::find(const string & label) const {
    for (Node * x = first; x!= NULL; x = x->next) {
        if (x->label == label) {
            return x->value;
        }
    }
    // precondition violated
    // TODO: Should throw an exception instead
    assertTrue(false);
}

template <typename T>
bool LList<T>::contains(const string & label) const {
    for (Node * x = first; x != NULL; x = x->next) {
        if (x->label == label) {
            return true;
        }
    }
    return false;
}

template <typename T>
int LList<T>::count() const {
    return size;
}

template <typename T>
void LList<T>::print() const {
    if (size() <= 0) {
        cout << "LList{}" << endl;
    } else {
        cout << "LList{";
        for (Node * x = first; x != NULL; x = x->next) {
            cout << " (" << x->label << ", " << &x->value << ")" ;
        }
        cout << " }" << endl;
    }
}

template <typename T>
auto_ptr< LList<int> > LList<T>::list2llist(const List<string> & list) {
    // A reference cannot be returned since LList is instanciated here.
    // LList could be returned by value but there's no copy constructor.
    // As such, the only way to return the created LList is with a pointer.

    // TODO: What about a smart pointer or authorizing copy ?

    // TODO: Actually could be a standalone function because it does not
    //       access the internal rep of LList.

    auto_ptr<LList<int> > llist(new LList<int>());
    auto_ptr<Generator<string> > g = list.elements();

    for (int i = 0; g->hasNext(); i++) {
        llist->add(g->next(), i);
    }

    return llist;
}

template <typename T>
void LList<T>::unlink(Node * x) {
    Node * next = x->next;
    Node * prev = x->prev;

    if (prev == NULL) {
        first = next;
    } else {
        prev->next = next;
        x->prev = NULL;
    }

    if (next == NULL) {
        last = prev;
    } else {
        next->prev = prev;
        x->next = NULL;
    }

    size--;
    delete x;
}

template <typename T>
void LList<T>::checkRep() const {
    if (doCheckRep) {
        assertTrue((first == NULL && last == NULL && size == 0)
            || (first != NULL && last != NULL && size > 0
                && first->prev == NULL && last->next == NULL));
    }
}

} // namespace util

#endif /* UTIL_LLIST_HPP */
