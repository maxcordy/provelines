/* 
 * StatePtrTable.hpp
 *
 * Created on August 22, 2013
 */

#ifndef STATEPTRTABLE_HPP
#define	STATEPTRTABLE_HPP

#include "core/fts/State.hpp"

#include "util/Uncopyable.hpp"
#include "util/Assert.hpp"
#include "util/Hashable.hpp"

#include <tr1/unordered_map>
#include <tr1/memory>

namespace core {
namespace checker {

/**
 * A StatePtrTable object is a mutable hashtable for shared pointer towards
 * State objects.
 * 
 * Computing hash codes and testing for equality is done using hashcode and
 * equals methods.
 * 
 * @bdawagne
 */
template<typename V>
class StatePtrTable : private util::Uncopyable {

private:
    class Hash {

    public:
        unsigned int operator() (const std::tr1::shared_ptr<const core::fts::State> & key) const {
            return key->hashCode();
        }

    }; // class Hash
    
    class Pred {

    public:
        bool operator() (
                const std::tr1::shared_ptr<const core::fts::State> & lhs,
                const std::tr1::shared_ptr<const core::fts::State> & rhs) const {
            
            return lhs->equals(*rhs);
        }

    }; // class Pred
    
    std::tr1::unordered_map<std::tr1::shared_ptr<const core::fts::State>, V, Hash, Pred> map;
    
    /*
     * Invariant Representation :
     *      I(c) = true;
     */

public:
    // Default constructor is fine
    
    // Default destructor is fine
    
    /**
     * @return true iff this contains a value at 'key'
     */
    bool contains(const std::tr1::shared_ptr<const core::fts::State> & key) const;
    
    /**
     * @modifies this
     * @effects Create a value at 'key' using the default constructor if none
     *          exists
     * @return A reference to the value at 'key'
     */
    V & operator[](const std::tr1::shared_ptr<const core::fts::State> & key);

    /**
     * @modifies this
     * @effects Remove the value at 'key' if such exits.
     */
    void remove(const std::tr1::shared_ptr<const core::fts::State> & key);  
    
private:
    /**
     * @effects Assert the invariant representation holds for this.
     */
    void checkRep() const;
        

}; // class StatePtrTable

template<typename V>
bool StatePtrTable<V>::contains(const std::tr1::shared_ptr<const core::fts::State > & key) const {
    return map.count(key) > 0;
}

template<typename V>
V & StatePtrTable<V>::operator[](const std::tr1::shared_ptr<const core::fts::State > & key) {
    V & result = map[key];
    
    checkRep();
    
    return result;
}

template<typename V>
void StatePtrTable<V>::remove(const std::tr1::shared_ptr<const core::fts::State > & key) {
    map.erase(key);
    
    checkRep();
}

template<typename V>
void StatePtrTable<V>::checkRep() const {
    if (doCheckRep) {
        // Nothing here
    }
}
    
} // namespace checker
} // namespace core



#endif	/* STATEPTRTABLE_HPP */

