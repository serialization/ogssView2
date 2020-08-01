//
// Created by feldentm on 01.08.20.
//

#include "Show.h"
std::string Show::toString(const ogss::fieldTypes::FieldType *t) {
    using namespace ogss::fieldTypes;

    // built-in types
    switch (t->typeID) {
    case ogss::KnownTypeID::BOOL:
        return "bool";
    case ogss::KnownTypeID::I8:
        return "i8";
    case ogss::KnownTypeID::I16:
        return "i16";
        break;
    case ogss::KnownTypeID::I32:
        return "i32";
    case ogss::KnownTypeID::I64:
        return "i64";
    case ogss::KnownTypeID::V64:
        return "v64";
    case ogss::KnownTypeID::F32:
        return "f32";
    case ogss::KnownTypeID::F64:
        return "f64";
    case ogss::KnownTypeID::ANY_REF:
        return "any";
    case ogss::KnownTypeID::STRING:
        return "string";

    default: {
        if (auto p = dynamic_cast<const ogss::AbstractPool *>(t)) {
            return *p->name;

        } else if (auto p =
                     dynamic_cast<const ogss::internal::AbstractEnumPool *>(
                       t)) {
            return *p->name;

        } else if (auto p =
                     dynamic_cast<const ArrayType<ogss::api::Box> *>(t)) {
            return toString(p->base) += "[]";

        } else if (auto p = dynamic_cast<const ListType<ogss::api::Box> *>(t)) {
            return "list<" + toString(p->base) + ">";

        } else if (auto p = dynamic_cast<const SetType<ogss::api::Box> *>(t)) {
            return "set<" + toString(p->base) + ">";

        } else if (auto p = dynamic_cast<
                     const MapType<ogss::api::Box, ogss::api::Box> *>(t)) {
            return "map<" + toString(p->keyType) + ", " +
                   toString(p->valueType) + ">";

        } else {
            return "type:" + std::to_string(t->typeID);
        }
    }
    }
}
std::string Show::toString(const ogss::internal::UnknownObject *t) {
    return *t->pool->name + std::string("#") +
           std::to_string(t->pool->getObjectID(t));
}
