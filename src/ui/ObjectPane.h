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

    //! human-readable presentation of field data
    //! @todo  this should be a list with typename on the left and a clickable
    //! value on the right that opens an ObjectPane if adequate
    wxTextCtrl *const fields;

    //! mid pane showing both directions of referral
    wxPanel *const referrals;

    //! right pane containing the export/show as graph UI
    wxPanel *const graph;

    //! TODO labels

    //! the maximum distance from the root to a node to be shown
    wxSlider *const maxDistance;

    //! the maximum distance from the root to a node to be shown
    //! @note we will count every value as node because it will occupy screen
    //! space
    wxSlider *const maxNodes;

    //! the maximum distance from the root to a node to be shown
    wxSlider *const maxContainerItems;

    //! create and open pdf
    wxButton *const showPDF;
    //! show in xdot
    wxButton *const showXDOT;

  public:
    ObjectPane(wxAuiNotebook *parent, const ogss::internal::UnknownObject *ref);

    std::string toString() final;

  private:
    void onShowXDOT(wxCommandEvent &event);
};

#endif // OGSS_VIEW_PP_OBJECTPANE_H
