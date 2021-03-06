//
// Created by feldentm on 21.07.20.
//

#include "TypePane.h"

#include "MainFrame.h"
#include "ObjectPane.h"
#include "Show.h"
#include "main.h"

#include <ogss/internal/UnknownObject.h>
#include <ogss/iterators/FieldIterator.h>
#include <wx/gtk/notebook.h>

/**
 * Allow tree entries to point to the ogss type representation.
 */
struct TypeEntry : public wxTreeItemData {
    ogss::fieldTypes::FieldType *const type;

    explicit TypeEntry(ogss::fieldTypes::FieldType *const type) : type(type) {}
};

wxColour blend(const wxColour &left, const wxColour &right) {
    int r = left.Red() + right.Red();
    int g = left.Green() + right.Green();
    int b = left.Blue() + right.Blue();
    return wxColour(r / 2, g / 2, b / 2);
}

TypePane::TypePane(wxAuiNotebook *parent) :
  TabEntry(parent),
  tree(new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                      wxTR_HAS_BUTTONS | wxTR_NO_LINES | wxTR_HIDE_ROOT)),
  root(tree->AddRoot("root")),
  type(new wxTextCtrl(this, wxID_ANY, "select a type", wxDefaultPosition,
                      wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY)),

  itemView(new wxPanel(this, wxID_ANY)),

  itemViewButtons(new wxPanel(itemView, wxID_ANY)),
  previous(new wxButton(itemViewButtons, wxID_ANY, "<<", wxDefaultPosition,
                        wxDefaultSize, wxBU_EXACTFIT)),
  itemPosition(new wxStaticText(
    itemViewButtons, wxID_ANY, "0XX", wxDefaultPosition, wxDefaultSize,
    wxST_NO_AUTORESIZE | wxALIGN_CENTRE_HORIZONTAL)),
  next(new wxButton(itemViewButtons, wxID_ANY, ">>", wxDefaultPosition,
                    wxDefaultSize, wxBU_EXACTFIT)),

  items(new wxListView(itemView, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                       wxLC_REPORT | wxLC_NO_HEADER | wxLC_SINGLE_SEL)),
  itemsOffset(0),
  maxItemOffset(0) {

    // add panel to notebook
    parent->InsertPage(0, this, "Types", true);

    // panel layout
    {
        auto sizer = new wxBoxSizer(wxHORIZONTAL);
        itemViewButtons->SetSizer(sizer);

        sizer->Add(previous, 0,
                   wxEXPAND | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 1);
        sizer->Add(next, 0, wxEXPAND | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL,
                   1);
        sizer->Add(itemPosition, 1, wxEXPAND | wxALL, 1);
    }
    {
        auto sizer = new wxBoxSizer(wxVERTICAL);
        itemView->SetSizer(sizer);

        sizer->Add(itemViewButtons, 0,
                   wxEXPAND | wxALIGN_TOP | wxALIGN_CENTER_HORIZONTAL, 1);
        sizer->Add(items, 1, wxEXPAND | wxALL, 1);
    }
    {
        auto sizer = new wxBoxSizer(wxHORIZONTAL);
        this->SetSizer(sizer);

        sizer->Add(tree, 1, wxEXPAND | wxALL, 0);
        sizer->AddSpacer(3);
        sizer->Add(type, 1, wxEXPAND | wxALL, 0);
        sizer->AddSpacer(3);
        sizer->Add(itemView, 1, wxEXPAND | wxALL, 0);
    }

    // for some reason type has by default the wrong style
    {
        const auto base =
          wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT);
        type->SetBackgroundColour(
          wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
        type->SetForegroundColour(base);

        classStyle.SetTextColour(blend(base, wxColour(0, 0, 220)));

        containerStyle.SetTextColour(blend(base, wxColour(220, 0, 0)));

        enumStyle.SetTextColour(blend(base, wxColour(220, 100, 0)));

        builtinStyle.SetTextColour(blend(base, wxColour(128, 220, 128)));

        inheritedStyle.SetTextColour(blend(base, wxColour(128, 128, 128)));
    }

    // bind events
    tree->Bind(wxEVT_TREE_SEL_CHANGED, &TypePane::onSelectionChanged, this);
    items->Bind(wxEVT_LIST_ITEM_ACTIVATED, &TypePane::onItemActivated, this);
    next->Bind(wxEVT_BUTTON, &TypePane::onNextItems, this);
    previous->Bind(wxEVT_BUTTON, &TypePane::onPreviousItems, this);
}

void TypePane::afterLoad() {
    tree->DeleteChildren(root);

    auto sg = wxGetApp().get();

    auto nodes = new std::unordered_map<ogss::AbstractPool *, wxTreeItemId>;

    for (auto t : sg->allClasses()) {

        auto target = nodes->find(t->super);
        wxTreeItemId parent;
        if (nodes->end() != target) {
            parent = target->second;
        } else {
            parent = root;
        }

        (*nodes)[t] =
          tree->AppendItem(parent,
                           wxString::Format("%s (%i/%i)", t->name->c_str(),
                                            t->staticSize(), t->size()),
                           -1, -1, new TypeEntry(t));
    }

    for (auto t : sg->allEnums()) {
        auto node =
          tree->AppendItem(root,
                           wxString::Format("enum %s (%li)", t->name->c_str(),
                                            t->end() - t->begin()),
                           -1, -1, new TypeEntry(t));
    }

    // note: it is currently non-trivial to add strings because they are too
    // lazy also, you barely ever need them

    for (auto t : sg->allContainers()) {
        tree->AppendItem(
          root, wxString::Format("%s (%li)", Show::toString(t), t->knownSize()),
          -1, -1, new TypeEntry(t));
    }

    delete nodes;
}

void TypePane::onSelectionChanged(wxCommandEvent &e) {
    type->Clear();

    // reset item list; max will be set by display
    itemsOffset = maxItemOffset = 0;

    auto entry =
      dynamic_cast<TypeEntry *>(tree->GetItemData(tree->GetSelection()));
    if (nullptr == entry) {
        // root selected
        type->AppendText("select a type");
    } else if (auto cls = dynamic_cast<ogss::AbstractPool *>(entry->type)) {
        displayClass(cls);
    } else if (auto cls = dynamic_cast<ogss::internal::AbstractEnumPool *>(
                 entry->type)) {
        displayEnum(cls);
    } else {
        type->AppendText(Show::toString(entry->type));
    }

    refillItems();
}

void TypePane::onItemActivated(wxListEvent &event) {
    auto *target =
      reinterpret_cast<ogss::internal::UnknownObject *>(event.GetData());

    if (target) {
        auto p = getParent();
        auto size = p->GetPageCount();
        p->InsertPage(size, new ObjectPane(p, target), Show::toString(target),
                      true);

    } else {
        // TODO create new tab and select it (non-objects)
        std::cout << "selected "
                  << "(something else)" << std::endl;
    }
}

void TypePane::displayClass(ogss::AbstractPool *t) {
    // show items in buckets of 100
    maxItemOffset = t->size() / 100;

    // TODO attributes

    type->AppendText(t->name->c_str());

    if (nullptr != t->super) {
        type->AppendText(" : ");

        show(t->super);
    }

    type->AppendText(" {\n");

    auto fs = t->allFields();
    while (fs.hasNext()) {
        auto f = fs.next();

        type->AppendText("\n   ");
        show(f->type);
        type->AppendText(" ");

        auto begin = type->GetLastPosition();
        type->AppendText(*f->name);
        auto end = type->GetLastPosition();
        if (f->owner != t)
            type->SetStyle(begin, end, inheritedStyle);

        type->AppendText(";\n");
    }

    type->AppendText("}");
}

void TypePane::displayEnum(ogss::internal::AbstractEnumPool *t) {
    // maxItemOffset = 0; show all at once anyway

    type->AppendText("enum ");
    show(t);
    type->AppendText(" {");
    bool first = true;
    for (auto e : *t) {
        type->AppendText(first ? "\n   " : ",\n   ");
        type->AppendText(*e->name);
        first = false;
    }
    type->AppendText("\n}");
}

void TypePane::show(const ogss::fieldTypes::FieldType *const t) {
    using namespace ogss::fieldTypes;

    // built-in types
    auto begin = type->GetLastPosition();
    type->AppendText(Show::toString(t));
    auto end = type->GetLastPosition();

    if (0 <= t->typeID && t->typeID <= ogss::KnownTypeID::STRING) {
        type->SetStyle(begin, end, builtinStyle);
    } else if (auto p = dynamic_cast<const ogss::AbstractPool *>(t)) {
        type->SetStyle(begin, end, classStyle);

    } else if (auto p =
                 dynamic_cast<const ogss::internal::AbstractEnumPool *>(t)) {
        type->SetStyle(begin, end, enumStyle);

    } else {
        type->SetStyle(begin, end, containerStyle);
    }
}

void TypePane::refillItems() {
    using namespace ogss::fieldTypes;

    itemPosition->SetLabel(std::to_string(itemsOffset) + "XX");

    items->ClearAll();
    items->InsertColumn(0, "items");

    auto entry =
      dynamic_cast<TypeEntry *>(tree->GetItemData(tree->GetSelection()));

    if (nullptr == entry) {
        // root selected
        type->AppendText("select a type");
    } else if (auto cls = dynamic_cast<ogss::AbstractPool *>(entry->type)) {

        // ok, so the only sane way to access objects if you are not the OGSS
        // implementation is to use the iterator and to just skip objects that
        // are not relevant
        auto all = cls->allObjects();

        int skip = 100 * itemsOffset;
        while (skip-- > 0 && all->hasNext()) {
            all->next();
        }

        if (all->hasNext()) {
            for (int i = 0; i < 100 && all->hasNext(); i++) {
                auto ref =
                  dynamic_cast<ogss::internal::UnknownObject *>(all->next());

                items->InsertItem(i, Show::toString(ref));
                items->SetItemPtrData(i, (size_t)ref);
            }
        }

    } else if (auto cls = dynamic_cast<ogss::internal::AbstractEnumPool *>(
                 entry->type)) {

        // we will always show all enums
        // how many enum entries can a single enum have, right?
        int i = 0;
        for (auto ref : *cls) {
            items->InsertItem(i, *ref->name);
            items->SetItemPtrData(i, 0);
            i++;
        }

    } else if (auto p =
                 dynamic_cast<const ArrayType<ogss::api::Box> *>(entry->type)) {

        items->InsertItem(0, "TODO arrays");

    } else if (auto p =
                 dynamic_cast<const ListType<ogss::api::Box> *>(entry->type)) {

        items->InsertItem(0, "TODO lists");

    } else if (auto p =
                 dynamic_cast<const SetType<ogss::api::Box> *>(entry->type)) {

        items->InsertItem(0, "TODO sets");

    } else if (auto p =
                 dynamic_cast<const MapType<ogss::api::Box, ogss::api::Box> *>(
                   entry->type)) {

        items->InsertItem(0, "TODO maps");
    }

    // resize column again to fit inserted elements
    items->SetColumnWidth(0, wxLIST_AUTOSIZE);
}
void TypePane::onNextItems(wxCommandEvent &event) {
    if (itemsOffset < maxItemOffset) {
        itemsOffset++;
        refillItems();
    }
}
void TypePane::onPreviousItems(wxCommandEvent &event) {
    if (0 < itemsOffset) {
        itemsOffset--;
        refillItems();
    }
}
