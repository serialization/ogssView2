//
// Created by feldentm on 21.07.20.
//

#ifndef OGSS_VIEW_PP_TYPEPANE_H
#define OGSS_VIEW_PP_TYPEPANE_H

#include "../empty/File.h"
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/wxprec.h>

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
    wxTextAttr classStyle;
    wxTextAttr containerStyle;
    wxTextAttr enumStyle;
    wxTextAttr builtinStyle;
    wxTextAttr inheritedStyle;

    //! contains the item UI elements
    wxPanel *const itemView;
    //! contains the buttons in itemView
    wxPanel *const itemViewButtons;

    //! show previous elements in item list
    wxButton *const previous;

    //! show the current position in the item list
    wxStaticText *const itemPosition;

    //! show next elements in item list
    wxButton *const next;

    //! a list of items showing up to 100 elements
    wxListView *const items;

    //! offset of items in 100 elements
    int itemsOffset;

  public:
    TypePane(MainFrame *parent);

    /**
     *  reload the data from a graph
     */
    void afterLoad();

  private:
    //! triggered, if a type is selected
    void onSelectionChanged(wxCommandEvent &e);

    //! triggered, if an object is selected
    void onItemSelected(wxListEvent& event);

    //! triggered, if an object is activated, i.e. enter/doubleclick
    void onItemActivated(wxListEvent& event);

    //! update type to show a representation of t
    void displayClass(ogss::AbstractPool *t);

    //! update type to show a representation of t
    void displayEnum(ogss::internal::AbstractEnumPool *t);

    //! add the image of t to the type view
    void show(const ogss::fieldTypes::FieldType *t);

    //! fill items with adequate entries
    void refillItems();
  public:
    //! create human-readable name for t
    static std::string toString(const ogss::fieldTypes::FieldType *t);
};

#endif // OGSS_VIEW_PP_TYPEPANE_H
