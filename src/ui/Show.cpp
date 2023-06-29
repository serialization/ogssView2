//
// Created by feldentm on 01.08.20.
//

#include "Show.h"
#include <iostream>
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

            if (nullptr == val.array)
                return "null";

            auto ss = new std::stringstream{};
            *ss << "[";
            auto first = true;
            auto all = val.array;

            for (int i = 0; i < all->length(); i++) {
                if (first)
                    first = false;
                else
                    *ss << ", ";

                *ss << toString(p->base, all->get(i));
            }

            *ss << "]";

            auto rval = std::string(ss->str());
            delete ss;

            return std::string(rval);

        } else if (auto p = dynamic_cast<const ListType<ogss::api::Box> *>(t)) {

            if (nullptr == val.list)
                return "null";

            auto ss = new std::stringstream{};
            *ss << "[";
            auto first = true;
            auto all = val.list;

            for (int i = 0; i < all->length(); i++) {
                if (first)
                    first = false;
                else
                    *ss << ", ";

                *ss << toString(p->base, all->get(i));
            }

            *ss << "]";

            auto rval = std::string(ss->str());
            delete ss;

            return std::string(rval);

        } else if (auto p = dynamic_cast<const SetType<ogss::api::Box> *>(t)) {

            if (nullptr == val.set)
                return "null";

            auto ss = new std::stringstream{};
            *ss << "{";
            auto first = true;
            auto all = val.set->all();

            while (all->hasNext()) {
                if (first)
                    first = false;
                else
                    *ss << ", ";

                *ss << toString(p->base, all->next());
            }

            *ss << "}";

            auto rval = std::string(ss->str());
            delete ss;

            return std::string(rval);

        } else if (auto p = dynamic_cast<
                     const MapType<ogss::api::Box, ogss::api::Box> *>(t)) {

            if (nullptr == val.map)
                return "null";

            auto ss = new std::stringstream{};
            *ss << "{";
            auto first = true;

            auto all = val.map->all();
            while (all->hasNext()) {
                if (first)
                    first = false;
                else
                    *ss << ", ";

                auto kv = all->next();
                *ss << toString(p->keyType, kv.first);
                *ss << " -> ";
                *ss << toString(p->valueType, kv.second);
            }

            *ss << "}";
            std::cout << ss->tellp() << std::endl;

            auto rval = std::string(ss->str());
            delete ss;

            return std::string(rval);

        } else {
            return "some value of type:" + std::to_string(t->typeID);
        }
    }
    }
}
