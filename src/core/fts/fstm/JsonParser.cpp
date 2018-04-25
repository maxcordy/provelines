/*
 * JsonAdapter.cpp
 *
 * Created on 2013-07-29
 */

#include "JsonParser.hpp"

#include "FstmTransition.hpp"
#include "core/features/FeatureExpGenVisitor.hpp"
#include "util/Assert.hpp"
#include "util/NotPossibleException.hpp"
#include "util/parser/BoolExp.hpp"
#include "util/parser/BoolExpParser.hpp"
#include "util/JsonException.hpp"

#include <json/json.h>

using core::features::BoolFeatureExpFactory;
using core::features::FeatureExpGenVisitor;
using core::features::FeatureExpGenVisitorPtr;
using core::features::FeatureExpPtr;
using std::istream;
using std::string;
using std::tr1::shared_ptr;
using util::Generator;
using util::JsonException;
using util::parser::BoolExpParser;
using util::parser::BoolExpPtr;
using util::parser::VisitorPtr;

namespace core {
namespace fts {
namespace fstm {

// constructors
JsonParser::JsonParser(istream & jsonFstm, const BoolFeatureExpFactory
        & featureExpFactory)
    : featureExpFactory(featureExpFactory), root() {
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(jsonFstm, root);
    if (!parsingSuccessful) {
        throw JsonException("JsonParser constructor");
    }
}

// public methods
int JsonParser::getNbOfTransitions() const {
    return root["FSTM"]["nbOfTransitions"].asInt();
}

FstmTransitionGeneratorPtr JsonParser::transitions() const {
    return FstmTransitionGeneratorPtr(
            new TransitionGenerator(root["transitions"], featureExpFactory));
}

int JsonParser::getNbOfProperties() const {
    return root["FSTM"]["nbOfLtlProperties"].asInt();
}

shared_ptr<Generator<string> > JsonParser::properties() const {
    return shared_ptr<Generator<string> >(
            new StringArrayGenerator(root["ltlProperties"]));
}

shared_ptr<Generator<string> > JsonParser::states() const {
    return shared_ptr<Generator<string> >(
            new StringArrayGenerator(root["states"]));
}

// transition generator
JsonParser::TransitionGenerator::TransitionGenerator(const Json::Value &
        transitions, const BoolFeatureExpFactory & featureExpFactory)
    : transitions(transitions), index(0),
        featureExpFactory(featureExpFactory) {}

bool JsonParser::TransitionGenerator::hasNext() const {
    return index < transitions.size();
}

FstmTransitionPtr JsonParser::TransitionGenerator::next() {
    assertTrue(hasNext());

    Json::Value transition = transitions[index];
    index++;

    BoolExpParser boolExpParser(transition["featureExp"].asString());

    return FstmTransitionPtr(new FstmTransition(
            transition["source"].asString(),
            transition["target"].asString(), transition["event"].asString(),
            toFeatureExp(boolExpParser.parse())));
}

FeatureExpPtr JsonParser::TransitionGenerator::toFeatureExp(const BoolExpPtr &
        boolExp) {
    FeatureExpGenVisitorPtr visitor(
            new FeatureExpGenVisitor(featureExpFactory));
    boolExp->accept(*visitor);
    return visitor->getFeatureExp();
}

// string array generator
JsonParser::StringArrayGenerator::StringArrayGenerator(const Json::Value &
        array) : array(array), index(0) {}

bool JsonParser::StringArrayGenerator::hasNext() const {
    return index < array.size();
}

string JsonParser::StringArrayGenerator::next() {
    assertTrue(hasNext());

    Json::Value element = array[index];
    index++;

    return element.asString();
}

} // namespace fstm
} // namespace fts
} // namespace core
