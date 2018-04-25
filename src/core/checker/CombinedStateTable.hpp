/* 
 * CombinedStateTable.hpp
 *
 * Created on August 14, 2013
 */

#ifndef COMBINEDSTATETABLE_HPP
#define	COMBINEDSTATETABLE_HPP

#include "CombinedState.hpp"

#include "util/Uncopyable.hpp"
#include "util/Assert.hpp"

#include <tr1/unordered_map>

namespace core {
namespace checker {

/**
 * A CombinedStateTable object is a mutable hashtable for CombinedState objects.
 * 
 * Computing hash codes and testing for equality is done using hashcode and
 * equals methods.
 * 
 * @bdawagne
 */
template<typename V>
class CombinedStateTable : private util::Uncopyable {

private:
    class Hash {

    public:
        unsigned int operator() (const CombinedState & key) const {
            return key.hashCode();
        }

    }; // class Hash
    
    class Pred {

    public:
        bool operator() (const CombinedState & lhs, const CombinedState & rhs) const {
            return lhs.equals(rhs);
        }

    }; // class Pred
    
    std::tr1::unordered_map<CombinedState, V, Hash, Pred> map;
    
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
    bool contains(const CombinedState & key) const;
    
    /**
     * @modifies this
     * @effects Create a value at 'key' using the default constructor if none
     *          exists.
     * @return A reference to the value at 'key'
     */
    V & operator[](const CombinedState & key);

    /**
     * @modifies this
     * @effects Remove the value at 'key' if such exits.
     */
    void remove(const CombinedState & key);  
    
private:
    /**
     * @effects Assert the invariant representation holds for this.
     */
    void checkRep() const;
        

}; // class CombinedStateTable

template<typename V>
bool CombinedStateTable<V>::contains(const CombinedState & key) const {
    return map.count(key) > 0;
}

template<typename V>
V & CombinedStateTable<V>::operator[](const CombinedState & key) {
    V & result = map[key];
    
    checkRep();
    
    return result;
}

template<typename V>
void CombinedStateTable<V>::remove(const CombinedState & key) {
    map.erase(key);
    
    checkRep();
}

template<typename V>
void CombinedStateTable<V>::checkRep() const {
    if (doCheckRep) {
        // Nothing here
    }
}
    
} // namespace checker
} // namespace core



#endif	/* COMBINEDSTATETABLE_HPP */

