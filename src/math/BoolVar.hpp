/*
 * BoolVar.hpp
 *
 * Created on 2013-07-16
 */

#ifndef MATH_BOOLVAR_HPP
#define MATH_BOOLVAR_HPP

#include <string>
#include <tr1/unordered_map>

namespace math {

/**
 * BoolVars are immutable boolean variables with a non-empty name and a
 * positive numeric id, and for each unique name and id there is at most
 * one boolean variable.
 *
 * @author bdawagne
 * @author mwi
 */
class BoolVar {

private:
    /** A table mapping names to ids. */
    static std::tr1::unordered_map<std::string, int> nameTable;
    /** A table mapping ids to names. */
    static std::tr1::unordered_map<int, std::string> idTable;
    /** The max id of the existing boolean variables. */
    static int maxId;

    const std::string name;
    const int id;

    /*
     * Representation Invariant:
     *   I(c) =
     *       { (name, id) | <name, id> in c.nameTable }
     *         = { (name, id) | <id, name> in c.idTable }
     *     AND
     *       for all <name, id> in c.nameTable
     *         .(id <= c.maxId && name.length > 0 && id > 0)
     *     AND
     *       <c.name, c.id> in c.nameTable
     */

    /**
     * @requires name.length > 0 && id > 0
     * @effects Make this be the boolean variable with name 'name' and
     *           id 'id'.
     */
    BoolVar(const std::string & name, int id);

public:
    // The default destructor is fine.

    /**
     * @return the name of this.
     */
    std::string getName() const;

    /**
     * @return the id of this.
     */
    int getId() const;

    /**
     * @requires name.length > 0
     * @return the boolean variable named 'name'.
     */
    static BoolVar makeBoolVar(const std::string & name);

    /**
     * @requires id > 0
     * @return the boolean variable identified by 'id'.
     */
    static BoolVar makeBoolVar(int id);

    /**
     * @requires name.length > 0 && id > 0 && there exists no boolean
     *            variable with name 'name' && there exists no boolean
     *            variable with id 'id'.
     * @return the new boolean variable with name 'name' and id 'id'.
     */
    static BoolVar createBoolVar(const std::string & name, int id);

private:
    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;

    /**
     * @return a name generated from the id 'id'.
     */
    static std::string generateName(int id);

    /**
     * @requires name.length > 0 && id > 0
     * @effects Saves the boolean variable with name 'name' and id 'id'.
     */
    static void save(const std::string & name, int id);
};

} // namespace math

#endif /* MATH_BOOLVAR_HPP */
