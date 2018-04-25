/*
 * List.hpp
 *
 * Created on 2013-07-05
 */

#ifndef UTIL_LIST_HPP
#define UTIL_LIST_HPP

#include "Assert.hpp"
#include "Generator.hpp"

#include <iostream>
#include <memory>

namespace util {

/**
 * Lists are mutable lists of elements of type T.
 *
 * Defensive copying is performed on the elements that are inserted. In case
 * of big objects, pointers can be stored instead. In this case the list
 * elements can be modified from the outside of the list.
 *
 * Testing for equality is done using the == operator. As such this operator
 * must be implemented by list elements.
 *
 * @author mwi
 */
template <typename T>
class List {

private:
    /** Nodes of the linked list. */
    struct Node {
        const T item;
        Node * next;
        Node * prev;

        Node(Node * prev, const T & element, Node * next)
            : item(element), next(next), prev(prev) {}
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
     * @effects Makes this be a new empty list.
     */
    explicit List();

    /**
     * @effects Makes this be a copy of 'list'.
     */
    List(const List & list);

    /**
     * @effects Destroys this.
     */
    ~List();

    /**
     * @modifies this
     * @effects Adds 'element' to the head of this.
     */
    void addFirst(const T & element);

    /**
     * @modifies this
     * @effects Adds 'element' to the end of this.
     */
    void addLast(const T & element);

    /**
     * @requires this not empty
     * @modifies this
     * @effects Removes the first element of this.
     * @return the removed element.
     */
    T removeFirst();

    /**
     * @requires this not empty
     * @modifies this
     * @effects Removes the last element of this.
     * @return the removed element.
     */
    T removeLast();

    /**
     * @modifies this
     * @effects Removes the first occurence of 'element' when traversing this
     *           from head to tail.
     * @return true iff 'element' was present in this.
     */
    bool removeFirstOccurence(const T & element);

    /**
     * @modifies this
     * @effects Removes the last occurence of 'elements' when traversing this
     *           from head to tail.
     * @return true iff 'element' was present in this.
     */
    bool removeLastOccurence(const T & element);

    /**
     * @modifies this, list
     * @effects Concats this with the elements in 'list', i.e., removes all
     *           the elements in 'list' and adds them at the end of this.
     */
    void concat(List & list);

    /**
     * @return the number of elements in this.
     */
    int count() const;

    /**
     * @return true iff 'element' is in this.
     */
    bool find(const T & element) const;

    /**
     * @requires this contains at least one element
     * @return the first element of this
     */
    T getFirst() const;

    /**
     * @requires this contains at least one element
     * @return the last element of this
     */
    T getLast() const;

    /**
     * @modifies stdout
     * @effects Prints this on stdout.
     */
    void print() const;

    /**
     * @return a pointer to a generator that will produce all the elements
     *         of this (as Ts), each exactly once, in the natural list
     *         order.
     * @requires this must not be modified while the generator is in use
     */
    std::auto_ptr<Generator<T> > elements() const;

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

    // Prevents the use of the assignment operator.
    List & operator=(const List &);

    /**
     * Generator of the elements of the list.
     */
    class ElementGenerator : public Generator<T> {

    private:
        Node * cur; // The current node being considered.

    public:
        /**
         * @effects Makes this be a new element generator with the first
         *           node 'first' of the list being iterated.
         */
        ElementGenerator(Node * first);

        // The default destructor is fine because 'cur' directly points to a
        // node of the linked list itself.

        // overridden methods
        bool hasNext() const;
        T next();

    }; // end ElementGenerator
};

template <typename T>
List<T>::List()
    : size(0), first(NULL), last(NULL) {
    checkRep();
}

template <typename T>
List<T>::List(const List & list)
    : size(0), first(NULL), last(NULL) {
    for (Node * x = list.first; x != NULL; x = x->next) {
        addLast(x->item);
    }
}

template <typename T>
List<T>::~List() {
    while (first != NULL) {
        Node * tmp = first;
        first = first->next;
        delete tmp;
    }
}

template <typename T>
void List<T>::addFirst(const T & value) {
    Node * const f = first;
    Node * const newNode = new Node(NULL, value, f);
    first = newNode;
    if (f == NULL) {
        last = newNode;
    } else {
        f->prev = newNode;
    }
    size++;

    checkRep();
}

template <typename T>
void List<T>::addLast(const T & value) {
    Node * const l = last;
    Node * const newNode = new Node(l, value, NULL);
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
T List<T>::removeFirst() {
    assertTrue(count() > 0);

    const T firstElement = first->item;
    unlink(first);

    checkRep();
    return firstElement;
}

template <typename T>
T List<T>::removeLast() {
    assertTrue(count() > 0);

    const T lastElement = last->item;
    unlink(last);

    checkRep();
    return lastElement;
}

template <typename T>
bool List<T>::removeFirstOccurence(const T & value) {
    bool found = false;
    for (Node * x = first; x != NULL; x = x->next) {
        if (value == x->item) {
            unlink(x);
            found = true;
            break;
        }
    }
    checkRep();
    return found;
}

template <typename T>
bool List<T>::removeLastOccurence(const T & value) {
    bool found = false;
    for (Node * x = last; x != NULL; x = x->prev) {
        if (value == x->item) {
            unlink(x);
            found = true;
            break;
        }
    }
    checkRep();
    return found;
}

template <typename T>
void List<T>::concat(List<T> & list) {
    int expectedSize = size + list.size;

    // chaining the two lists together
    last->next = list.first;
    list.first->prev = last;
    // setting the new last node
    last = list.last;
    // setting the new size
    size += list.size;

    // emptying 'list'
    list.first = NULL;
    list.last = NULL;
    list.size = 0;
    list.checkRep();

    assertTrue(size == expectedSize);
    checkRep();
}

template <typename T>
int List<T>::count() const {
    return size;
}

template <typename T>
bool List<T>::find(const T & element) const {
    for (Node * x = first; x != NULL; x = x->next) {
        if (x->item == element) {
            return true;
        }
    }
    return false;
}

template <typename T>
T List<T>::getFirst() const {
    assertTrue(size > 0);
    return first->item;
}

template <typename T>
T List<T>::getLast() const {
    assertTrue(size > 0);
    return last->item;
}

template <typename T>
void List<T>::print() const {
    using std::cout;
    using std::endl;
    if (count() <= 0) {
        cout << "List{}" << endl;
    } else {
        cout << "List{";
        for (Node * x = first; x != NULL; x = x->next) {
            cout << " (" << &x->item << ")" ;
        }
        cout << " }" << endl;
    }
}

template <typename T>
std::auto_ptr<Generator<T> > List<T>::elements() const {
    return std::auto_ptr< Generator<T> >(new ElementGenerator(first));
}

template <typename T>
void List<T>::unlink(Node * x) {
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
void List<T>::checkRep() const {
    if (doCheckRep) {
        assertTrue((first == NULL && last == NULL && size == 0)
           || (first != NULL && last != NULL && size > 0
                && first->prev == NULL && last->next == NULL));
    }
}

template <typename T>
List<T>::ElementGenerator::ElementGenerator(Node* first)
    : cur(first) {}

template <typename T>
bool List<T>::ElementGenerator::hasNext() const {
    return cur != NULL;
}

template <typename T>
T List<T>::ElementGenerator::next() {
    assertTrue(hasNext());
    Node * tmp = cur;
    cur = cur->next;
    return tmp->item;
}

} // namespace util

#endif /* UTIL_LIST_HPP */
