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

    for (auto t : *sg) {
        auto target = nodes->find(t->super);
        wxTreeItemId parent;
        if (nodes->end() != target) {
            parent = target->second;
        } else {
            parent = root;
        }
        (*nodes)[t] = tree->AppendItem(parent, t->name->c_str());
    }

    delete nodes;
}
