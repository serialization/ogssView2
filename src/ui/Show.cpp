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
    if (t)
        return *t->pool->name + std::string("#") +
               std::to_string(t->pool->getObjectID(t));
    else
        return std::string("null");
}

std::string Show::toString(const ogss::fieldTypes::FieldType *t,
                           ogss::api::Box val) {
    using namespace ogss::fieldTypes;

    // built-in types
    switch (t->typeID) {
    case ogss::KnownTypeID::BOOL:
        return val.boolean ? "true" : "false";

    case ogss::KnownTypeID::I8:
        return std::to_string(val.i8);

    case ogss::KnownTypeID::I16:
        return std::to_string(val.i16);

    case ogss::KnownTypeID::I32:
        return std::to_string(val.i32);

    case ogss::KnownTypeID::I64:
    case ogss::KnownTypeID::V64:
        return std::to_string(val.i64);

    case ogss::KnownTypeID::F32:
        return std::to_string(val.f32);

    case ogss::KnownTypeID::F64:
        return std::to_string(val.f64);

    case ogss::KnownTypeID::ANY_REF:
        return toString((ogss::internal::UnknownObject *)val.anyRef);

    case ogss::KnownTypeID::STRING:
        if (val.string)
            return std::string("\"") + *val.string + std::string("\"");
        else
            return std::string("null");

    default: {
        if (auto p = dynamic_cast<const ogss::AbstractPool *>(t)) {
            return toString((ogss::internal::UnknownObject *)val.anyRef);

        } else if (auto p =
                     dynamic_cast<const ogss::internal::AbstractEnumPool *>(
                       t)) {
            return *val.enumProxy->name;

        } else if (auto p =
                     dynamic_cast<const ArrayType<ogss::api::Box> *>(t)) {
            return "some array???";

        } else if (auto p = dynamic_cast<const ListType<ogss::api::Box> *>(t)) {
            return "some list???";

        } else if (auto p = dynamic_cast<const SetType<ogss::api::Box> *>(t)) {
            return "some set???";

        } else if (auto p = dynamic_cast<
                     const MapType<ogss::api::Box, ogss::api::Box> *>(t)) {
            return "some set???";

        } else {
            return "some value of type:" + std::to_string(t->typeID);
        }
    }
    }
}
