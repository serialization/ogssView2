//
// Created by feldentm on 29.07.20.
//

#ifndef OGSS_VIEW_PP_TABENTRY_H
#define OGSS_VIEW_PP_TABENTRY_H

#include <wx/aui/auibook.h>
#include <wx/notebook.h>
#include <wx/wx.h>

/**
 * The base of all classes that will ever be used as tab contents of the main
 * window.
 */
class TabEntry : public wxPanel {
  protected:
    explicit TabEntry(wxAuiNotebook *parent);

    wxAuiNotebook *getParent() const {
        return dynamic_cast<wxAuiNotebook *>(wxPanel::GetParent());
    }

  public:
    //! return a human readable representation of the current content that will
    //! be shown in the status bar
    virtual std::string toString() = 0;
};

#endif // OGSS_VIEW_PP_TABENTRY_H
