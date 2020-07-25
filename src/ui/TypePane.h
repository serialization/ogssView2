//
// Created by feldentm on 21.07.20.
//

#ifndef OGSS_VIEW_PP_TYPEPANE_H
#define OGSS_VIEW_PP_TYPEPANE_H

#include <wx/wxprec.h>
#include <wx/treectrl.h>
#include "../empty/File.h"

#ifndef WX_PRECOMP

#include <wx/wx.h>

#endif

class MainFrame;

/**
 * A pane that shows the type hierarchy of the current file.
 *
 * @note this component can only be created if a file has been loaded
 */
class TypePane {
    //! container of the pane
    wxPanel *const panel;

    //! all user-defined types in the file
    wxTreeCtrl *const tree;
    //! the root of tree
    const wxTreeItemId root;

    //! the presentation of the selected type
    wxTextCtrl *const type;
    wxTextAttr linkStyle;
public:

    TypePane(MainFrame *parent);

    /**
     *  reload the data from a graph
     */
    void afterLoad();

private:
    void onSelectionChanged(wxCommandEvent &e);

    //! update type to show a representation of type
    void displayClass(ogss::AbstractPool* t);
};


#endif //OGSS_VIEW_PP_TYPEPANE_H
