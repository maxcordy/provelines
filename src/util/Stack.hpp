/*
 * Stack.hpp
 *
 * Created on 2013-07-15
 */

#ifndef UTIL_STACK_HPP
#define UTIL_STACK_HPP

#include <memory>

#include "Assert.hpp"
#include "Uncopyable.hpp"
#include "List.hpp"

namespace util {

/**
 * Stacks are mutable stacks of elements of type T.
 *
 * Defensive copying is performed on the elements that are inserted. In case
 * of big objects, pointers can be stored instead. In this case the stack
 * elements can be modified from the outside of the stack.
 *
 * Testing for equality is done using the == operator. As such this operator
 * must be implemented by stack elements.
 *
 * @author bdawagne
 */
template <typename T>
class Stack : private Uncopyable {

private:
    List<T> list;

    /*
     * Representation Invariant:
     *   I(c) = true
     */

public:
    /**
     * @effects Makes this be a new empty stack.
     */
    Stack();

    // The default destructor is fine.

    /**
     * @modifies this
     * @effects Adds 'element' at the top of this.
     */
    void push(const T & element);

    /**
     * @modifies this
     * @effects Removes the top element from this.
     * @return the element at the top of this.
     */
    T pop();

    /**
     * @return the element at the top of this.
     */
    T peek() const;

   /**
    * @return true iff 'element' is in this.
    */
    bool find(const T & element) const;

    /**
     * @return true iff this is empty, i.e., this contains no element.
     */
    bool empty() const;

    /**
     * @return the number of elements in this.
     */
    int count() const;

    /**
     * @return a pointer to a generator that will produce all the elements
     *          of this (as Ts), each exactly once, from the bottom to the top
     *          of this.
     * @requires this must not be modified while the generator is in use
     */
    std::auto_ptr<Generator<T> > elements() const;

private:
    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;
};

template <typename T>
Stack<T>::Stack()
    : list() {
    checkRep();
}

template <typename T>
void Stack<T>::push(const T & element) {
    list.addLast(element);
}

template <typename T>
T Stack<T>::pop() {
    return list.removeLast();
}

template <typename T>
T Stack<T>::peek() const {
    return list.getLast();
}

template <typename T>
bool Stack<T>::find(const T & element) const {
    return list.find(element);
}

template <typename T>
bool Stack<T>::empty() const {
    return (list.count() <= 0);
}

template <typename T>
int Stack<T>::count() const {
    return list.count();
}

template <typename T>
std::auto_ptr<Generator<T> > Stack<T>::elements() const {
    return list.elements();
}

template <typename T>
void Stack<T>::checkRep() const {
    if (doCheckRep) {
        assertTrue(true);
    }
}

} // namespace util

#endif /* UTIL_STACK_HPP */
