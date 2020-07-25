//
// Created by feldentm on 21.07.20.
//

#include <ogss/iterators/FieldIterator.h>

#include "TypePane.h"
#include "MainFrame.h"
#include "main.h"

/**
 * Allow tree entries to point to the ogss type representation.
 */
struct TypeEntry : public wxTreeItemData {
    ogss::fieldTypes::FieldType *const type;

    explicit TypeEntry(ogss::fieldTypes::FieldType *const type) : type(type) {
    }
};

TypePane::TypePane(MainFrame *parent) : panel(new wxPanel(parent, wxID_ANY)),
                                        tree(new wxTreeCtrl(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                                            wxTR_HAS_BUTTONS | wxTR_NO_LINES | wxTR_HIDE_ROOT)),
                                        root(tree->AddRoot("root")),
                                        type(new wxTextCtrl(panel, wxID_ANY, "select a type", wxDefaultPosition,
                                                            wxDefaultSize,
                                                            wxTE_MULTILINE | wxTE_READONLY)) {

    parent->GetSizer()->Add(panel, 1, wxEXPAND | wxALL, 1);

    // panel layout
    {
        auto sizer = new wxBoxSizer(wxHORIZONTAL);
        panel->SetSizer(sizer);

        sizer->Add(tree, 1, wxEXPAND | wxALL, 1);
        sizer->AddSpacer(10);
        sizer->Add(type, 1, wxEXPAND | wxALL, 1);
    }

    // for some reason type has by default the wrong style
    {
        type->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
        type->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT));

        linkStyle.SetTextColour(wxColor(0, 0, 220));
        linkStyle.SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
        linkStyle.SetFontStyle(wxFONTSTYLE_ITALIC);
    }

    // on select type
    tree->Bind(
            wxEVT_TREE_SEL_CHANGED, &TypePane::onSelectionChanged, this);
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

        (*nodes)[t] = tree->AppendItem(parent,
                                       wxString::Format("%s (%i/%i)", t->name->c_str(), t->staticSize(), t->size()),
                                       -1, -1,
                                       new TypeEntry(t));
    }

    for (auto t : sg->allEnums()) {
        auto node = tree->AppendItem(root, wxString::Format("enum %s (%li)", t->name->c_str(), t->end() - t->begin()),
                                     -1, -1,
                                     new TypeEntry(t));

        for (auto e : *t) {
            tree->AppendItem(node, wxString(e->name->c_str()));
        }
    }

    // note: it is currently non-trivial to add strings because they are too lazy
    // also, you barely ever need them

    for (auto t :sg->allContainers()) {
        tree->AppendItem(root, wxT("container typeID=") + wxString(std::to_string(t->typeID)), -1, -1,
                         new TypeEntry(t));
    }

    delete nodes;
}

void TypePane::onSelectionChanged(wxCommandEvent &e) {
    type->Clear();

    auto entry = dynamic_cast<TypeEntry *>(tree->GetItemData(tree->GetSelection()));
    if (nullptr == entry) {
        // root selected
        type->AppendText("select a type");
    } else if (auto cls = dynamic_cast<ogss::AbstractPool * >(entry->type)) {
        displayClass(cls);
    } else {
        type->AppendText("TODO: typeID=" + std::to_string(entry->type->typeID));
    }
}

void TypePane::displayClass(ogss::AbstractPool *t) {
    // TODO attributes

    type->AppendText(t->name->c_str());

    if (nullptr != t->super) {
        type->AppendText(" : ");
        auto name = t->super->name->c_str();
        auto begin = type->GetLastPosition();
        type->AppendText(name);
        auto end = type->GetLastPosition();
        type->SetStyle(begin, end, linkStyle);
    }

    type->AppendText(" {\n");

    auto fs = t->allFields();
    while(fs.hasNext()){
        auto f = fs.next();

        // TODO print field type
        type->AppendText("   typeID:" + std::to_string(f->type->typeID) + " ");
        type->AppendText(*f->name + ";\n");
    }

    type->AppendText("}");
}
