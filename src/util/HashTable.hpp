/*
 * HashTable.hpp
 *
 * Created on 2013-07-10
 */

#ifndef UTIL_HASHTABLE_HPP
#define UTIL_HASHTABLE_HPP

#include "Generator.hpp"
#include "Hashable.hpp"
#include "Uncopyable.hpp"

#include <memory>

namespace util {

/**
 * HashTable is an interface for a mutable hash table holding entries
 * <k,[v0...vn]> where k is an hashable key and [v0...vn] is a non-empty list
 * of values of type V.
 *
 * Testing for equality is done using the == operator. As a consequence this
 * operator must be implemented by values of the hash table.
 *
 * @author mwi
 */
template <typename K, typename V>
class HashTable : private Uncopyable {

public:
    /**
     * @modifies this
     * @effects Inserts 'value' at 'key' in this.
     */
    virtual void insert(const Hashable<K> & key, const V & value) = 0;

    /**
     * @modifies this
     * @effects Removes 'value' at 'key' from this if present, else does
     *           nothing.
     * @return true iff 'value' at 'key' is in this.
     */
    virtual bool remove(const Hashable<K> & key, const V & value) = 0;

    /**
     * @modifies this
     * @effects Removes the entry <key,[...]> from this if key is present.
     * @return true iff key is in this.
     */
    virtual bool removeEntry(const Hashable<K> & key) = 0;

    /**
     * @return a generator that produces the values [v0...vn] at 'key' in
     *          this (as Vs), each exactly once, in arbitrary order.
     * @requires this must not be modified while the generator is in use
     */
    virtual std::auto_ptr<Generator<V> > search(const Hashable<K> & key) const
        = 0;

    /**
     * @return true if 'value' at 'key' is found in this, else returns
     *          false.
     */
    virtual bool find(const Hashable<K> & key, const V & value) const = 0;

    /**
     * @return the number of values in this.
     */
    virtual int count() const = 0;

    // mandatory virtual destructor
    virtual ~HashTable() {}
};

} // namespace util

#endif
