//
// Created by feldentm on 01.08.20.
//

#ifndef OGSS_VIEW_PP_SHOW_H
#define OGSS_VIEW_PP_SHOW_H

#include "../empty/File.h"

#include <ogss/internal/UnknownObject.h>

/**
 * Several helper functions to show OGSS data to the user.
 */
struct Show {
    //! create human-readable name for t
    static std::string toString(const ogss::fieldTypes::FieldType *t);

    //! create a type#ID presentation of ref
    static std::string toString(const ogss::internal::UnknownObject *t);

    //! display arbitrary field contents
    static std::string toString(const ogss::fieldTypes::FieldType* ft, ogss::api::Box val);
};

#endif // OGSS_VIEW_PP_SHOW_H
