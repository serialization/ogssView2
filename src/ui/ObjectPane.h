//
// Created by feldentm on 01.08.20.
//

#ifndef OGSS_VIEW_PP_OBJECTPANE_H
#define OGSS_VIEW_PP_OBJECTPANE_H

#include "../empty/File.h"
#include "TabEntry.h"

#include <ogss/internal/UnknownObject.h>

/**
 * A text field on the left side showing the data stored in the fields of the
 * object. In the middle there shall be a list of "referred from" and a tree
 * view "refers to". On the right side UI to show a graphical representation is
 * shown. For now, we will stick with an XDot export.
 */
class ObjectPane final : public TabEntry {
    const ogss::internal::UnknownObject *const ref;

    // human-readable presentation of field data
    wxTextCtrl *const fields;

  public:
    ObjectPane(wxNotebook *parent, const ogss::internal::UnknownObject *ref);

    std::string toString() final;
};

#endif // OGSS_VIEW_PP_OBJECTPANE_H
