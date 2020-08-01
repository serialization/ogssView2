//
// Created by feldentm on 01.08.20.
//

#ifndef OGSS_VIEW_PP_OBJECTPANE_H
#define OGSS_VIEW_PP_OBJECTPANE_H

#include "../empty/File.h"
#include "TabEntry.h"

#include <ogss/internal/UnknownObject.h>

class ObjectPane final : public TabEntry {
    const ogss::internal::UnknownObject *const ref;

  public:
    ObjectPane(wxNotebook *parent,
               const ogss::internal::UnknownObject *const ref) :
      TabEntry(parent), ref(ref) {}

    std::string toString() final;
};

#endif // OGSS_VIEW_PP_OBJECTPANE_H
