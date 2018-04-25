/*
 * forwards.hpp
 *
 * Created on 2013-08-07
 */

#ifndef CORE_FEATURES_FORWARDS_HPP
#define	CORE_FEATURES_FORWARDS_HPP

#include <tr1/memory>

namespace core {
namespace features {

// Forward declarations
class BoolFeatureExp;
class BoolFeatureExpFactory;
class FeatureExp;
class FeatureExpGenVisitor;

// Smart pointer typedefs
typedef std::tr1::shared_ptr<BoolFeatureExp> BoolFeatureExpPtr;
typedef std::tr1::shared_ptr<BoolFeatureExpFactory> BoolFeatureExpFactoryPtr;
typedef std::tr1::shared_ptr<FeatureExp> FeatureExpPtr;
typedef std::tr1::shared_ptr<FeatureExpGenVisitor> FeatureExpGenVisitorPtr;

} // namespace features
} // namespace core

#endif	/* CORE_FEATURES_FORWARDS_HPP */
