/*
 * TVLModel.hpp
 *
 * Created on 2013-07-24
 */

#ifndef CORE_FD_TVL_TVLMODEL_HPP
#define	CORE_FD_TVL_TVLMODEL_HPP

#include "core/fd/FD.hpp"

#include "core/features/FeatureExp.hpp"
#include "core/features/BoolFeatureExpFactory.hpp"

#include "math/BoolFctFactory.hpp"

#include <string>

namespace core {
namespace fd {
namespace tvl {

/**
 * A TVLModel is a TVL implementation of the FD interface.
 *
 * @author bdawagne
 */
class TVLModel : public core::fd::FD {

private:
    static const char* WC_PATH;

    static const char* CLAUSES_PATH;

    static const char* MAPPING_PATH;

    static const char* COMMAND;

    static const char* TVL_HOME_VAR;

    char* TVLhome;

    std::tr1::shared_ptr<math::BoolFctFactory> & fctFactory;
    std::tr1::shared_ptr<core::features::BoolFeatureExpFactory> & expFactory;

    /*
     * Representation Invariant
     *   I(c) = true
     */

public:
    /**
     * @requires 'stream' is a valid TVL model stream
     * @effects Make this be the TVL model contained in the stream 'stream'
     */
    TVLModel(
            std::istream & stream,
            std::tr1::shared_ptr<math::BoolFctFactory> & fctFactory,
            std::tr1::shared_ptr<core::features::BoolFeatureExpFactory> & expFactory);

    /**
     * @requires 'stream' is a valid TVL model stream
     * @effects Make this be the TVL model contained in the stream 'stream'
     *          on which the filter 'filter' has been applied.
     */
    TVLModel(
            std::istream & stream,
            const std::string & filter,
            std::tr1::shared_ptr<math::BoolFctFactory> & fctFactory,
            std::tr1::shared_ptr<core::features::BoolFeatureExpFactory> & expFactory);

    // Default destructor is fine

    // overridden methods
    core::features::FeatureExp * toFeatureExp() const;

private:
    /**
     * @effects Applies a filter to the working copy of the TVL model file.
     */
    void addFilter(const std::string & filter);

    /**
     * @effects Generates the clause dimacs file and the map file from the
     *          working copy of the TVL model file.
     */
    void generateDimacsFiles() const;

    /**
     * @effects Creates boolean variables from the generated map file.
     */
    void createBoolVars() const;

    /**
     * @effects Asserts the rep invariant holds for this.
     */
    void checkRep() const;
};

} // namespace tvl
} // namespace fd
} // namespace core

#endif	/* CORE_FD_TVL_TVLMODEL_HPP */
