//
// Created by feldentm on 21.07.20.
//

#ifndef OGSS_VIEW_PP_TYPEPANE_H
#define OGSS_VIEW_PP_TYPEPANE_H

#include <wx/wxprec.h>
#include <wx/treectrl.h>

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
    wxPanel *const panel;
    wxTreeCtrl *const tree;
    const wxTreeItemId root;
public:

    TypePane(MainFrame *parent);

    /**
     *  reload the data from a graph
     */
    void afterLoad();
};


#endif //OGSS_VIEW_PP_TYPEPANE_H
