//
// Created by feldentm on 21.07.20.
//

#include "TypePane.h"
#include "MainFrame.h"
#include "main.h"


TypePane::TypePane(MainFrame *parent) : panel(new wxPanel(parent, wxID_ANY)),
                                        tree(new wxTreeCtrl(panel, wxID_ANY, wxDefaultPosition,
                                                            wxSize(400, panel->GetMaxHeight()),
                                                            wxTR_HAS_BUTTONS | wxTR_NO_LINES | wxTR_HIDE_ROOT)),
                                        root(tree->AddRoot("root")) {

    parent->GetSizer()->Add(panel, 1, wxEXPAND | wxALL, 1);

    auto sizer = new wxBoxSizer(wxHORIZONTAL);
    panel->SetSizer(sizer);

    sizer->Add(tree, 1, wxEXPAND | wxALL, 1);
}

void TypePane::afterLoad() {
    tree->DeleteChildren(root);

    auto sg = wxGetApp().get();

    auto nodes = new std::unordered_map<ogss::AbstractPool *, wxTreeItemId>;

    for (auto t : sg->allClasses()) {
        std::cout << *t->name << std::endl;

        auto target = nodes->find(t->super);
        wxTreeItemId parent;
        if (nodes->end() != target) {
            parent = target->second;
        } else {
            parent = root;
        }

        (*nodes)[t] = tree->AppendItem(parent,
                                       wxString::Format("%s (%i/%i)", t->name->c_str(), t->staticSize(), t->size()));
    }

    for (auto t : sg->allEnums()) {
        auto node = tree->AppendItem(root, wxString::Format("enum %s (%li)", t->name->c_str(), t->end() - t->begin()));

        for (auto e : *t) {
            tree->AppendItem(node, wxString(e->name->c_str()));
        }
    }

    for (auto t : sg->allContainers()) {
        tree->AppendItem(root, wxT("container typeID=") + wxString(std::to_string(t->typeID)));
    }

    delete nodes;
}
