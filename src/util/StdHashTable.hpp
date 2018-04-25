/*
 * StdHashTable.hpp
 *
 * Created on 2013-07-11
 */

#ifndef UTIL_STDHASHTABLE_HPP
#define	UTIL_STDHASHTABLE_HPP

#include "HashTable.hpp"

#include "List.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <tr1/unordered_map>

namespace util {

/**
 * StdHashTable is an implementation of the HashTable interface using the
 * standard library.
 *
 * @author bdawagne
 * @author mwi
 */
template <typename K, typename V>
class StdHashTable : public HashTable<K, V> {

private:
    // Needed in order to avoid some compilation error.
    typedef typename std::tr1::unordered_map<unsigned int, List<V> *>
        ::const_iterator iterator;

    std::tr1::unordered_map<unsigned int, List<V>* > map;
    int nbOfValues;
    const List<V> EmptyList;

    /*
     * Representation Invariant:
     *   I(c) = c.nbOfValues >= 0 && c.EmptyList.size == 0
     */

public:
    /**
     * @effects Makes this be a new empty hash table.
     */
    StdHashTable();

    /**
     * @effects Destroys this.
     */
    ~StdHashTable();

    // overridden methods
    void insert(const Hashable<K> & key, const V & value);
    bool remove(const Hashable<K> & key, const V & value);
    bool removeEntry(const Hashable<K> & key);
    std::auto_ptr<Generator<V> > search(const Hashable<K> & key) const;
    bool find(const Hashable<K> & key, const V & value) const;
    int count() const;

private:
    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;
};

template <typename K, typename V>
StdHashTable<K, V>::StdHashTable()
    : map(), nbOfValues(0), EmptyList() {
    checkRep();
}

template <typename K, typename V>
StdHashTable<K, V>::~StdHashTable() {
    for (iterator it = map.begin(); it != map.end(); it++) {
        delete it->second;
    }
    map.clear();
}

template <typename K, typename V>
void StdHashTable<K, V>::insert(const Hashable<K> & key, const V & value) {
    iterator it(map.find(key.hashCode()));

    if (it != map.end()) { // key exists
        List<V> * const list = it->second;
        list->addLast(value);
    } else { // key doesn't exist
        List<V> * const list = new List<V>();
        list->addLast(value);
        std::pair<unsigned int, List<V> *> pair(key.hashCode(), list);
        map.insert(pair);
    }
    nbOfValues++;

    checkRep();
}

template <typename K, typename V>
bool StdHashTable<K, V>::remove(const Hashable<K> & key, const V & value) {
    bool removed = false;
    iterator it(map.find(key.hashCode()));

    if (it != map.end()) { // key exists
        List<V> * const list = it->second;
        if (list->removeFirstOccurence(value)) {
            if (list->count() <= 0) {
                map.erase(key.hashCode());
                delete list;
            }
            nbOfValues--;
            removed = true;
        }
    }

    checkRep();
    return removed;
}

template <typename K, typename V>
bool StdHashTable<K, V>::removeEntry(const Hashable<K> & key) {
    bool removed = false;
    iterator it(map.find(key.hashCode()));

    if (it != map.end()) { // key exists
        List<V> * const list = it->second;
        nbOfValues -= list->count();
        map.erase(key.hashCode());
        delete list;
        removed = true;
    }

    checkRep();
    return removed;
}

template <typename K, typename V>
std::auto_ptr<util::Generator<V> > StdHashTable<K, V>::search(const Hashable<K>
        & key) const {
    iterator it(map.find(key.hashCode()));

    if (it != map.end()) { // key exists
        const List<V> * const list = it->second;
        return list->elements();
    } else {
        return EmptyList.elements();
    }
}

template <typename K, typename V>
bool StdHashTable<K, V>::find(const Hashable<K> & key, const V & evalue) const {
    iterator it(map.find(key.hashCode()));

    if (it != map.end()) {
        const List<V> * const list = it->second;
        return list->find(evalue);
    } else {
        return false;
    }
}

template <typename K, typename V>
int StdHashTable<K, V>::count() const {
    return nbOfValues;
}

template <typename K, typename V>
void StdHashTable<K, V>::checkRep() const {
    if (doCheckRep) {
        assertTrue(nbOfValues >= 0);
        assertTrue(EmptyList.count() == 0);
    }
}

} // namespace util

#endif	/* UTIL_STDHASHTABLE_HPP */
