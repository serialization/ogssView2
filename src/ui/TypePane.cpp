//
// Created by feldentm on 21.07.20.
//

#include "TypePane.h"

#include "MainFrame.h"
#include "main.h"

#include <ogss/iterators/FieldIterator.h>
#include <ogss/fieldTypes/BuiltinFieldType.h>

/**
 * Allow tree entries to point to the ogss type representation.
 */
struct TypeEntry : public wxTreeItemData {
    ogss::fieldTypes::FieldType *const type;

    explicit TypeEntry(ogss::fieldTypes::FieldType *const type) : type(type) {
    }
};

wxColour blend(const wxColour &left, const wxColour &right) {
    int r = left.Red() + right.Red();
    int g = left.Green() + right.Green();
    int b = left.Blue() + right.Blue();
    return wxColour(r / 2, g / 2, b / 2);
}

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
        const auto base = wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT);
        type->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
        type->SetForegroundColour(base);

        classStyle.SetTextColour(blend(base, wxColour(0, 0, 220)));

        containerStyle.SetTextColour(blend(base, wxColour(220, 0, 0)));

        enumStyle.SetTextColour(blend(base, wxColour(220, 100, 0)));

        builtinStyle.SetTextColour(blend(base, wxColour(128, 220, 128)));

        inheritedStyle.SetTextColour(blend(base, wxColour(128, 128, 128)));
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
        tree->AppendItem(root, toString(t), -1, -1, new TypeEntry(t));
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
        type->AppendText(toString(entry->type));
    }
}


void TypePane::displayClass(ogss::AbstractPool *t) {
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


void TypePane::show(const ogss::fieldTypes::FieldType *const t) {
    using namespace ogss::fieldTypes;

    // built-in types
    auto begin = type->GetLastPosition();
    type->AppendText(toString(t));
    auto end = type->GetLastPosition();

    if (0 <= t->typeID && t->typeID <= ogss::KnownTypeID::STRING) {
        type->SetStyle(begin, end, builtinStyle);
    } else if (auto p = dynamic_cast<const ogss::AbstractPool *>(t)) {
        type->SetStyle(begin, end, classStyle);

    } else if (auto p = dynamic_cast<const ogss::internal::AbstractEnumPool *>(t)) {
        type->SetStyle(begin, end, enumStyle);

    } else {
        type->SetStyle(begin, end, containerStyle);
    }
}

std::string TypePane::toString(const ogss::fieldTypes::FieldType *t) {
    using namespace ogss::fieldTypes;

    // built-in types
    switch (t->typeID) {
        case ogss::KnownTypeID::BOOL:
            return "bool";
        case ogss::KnownTypeID::I8:
            return "i8";
        case ogss::KnownTypeID::I16:
            return "i16";
            break;
        case ogss::KnownTypeID::I32:
            return "i32";
        case ogss::KnownTypeID::I64:
            return "i64";
        case ogss::KnownTypeID::V64:
            return "v64";
        case ogss::KnownTypeID::F32:
            return "f32";
        case ogss::KnownTypeID::F64:
            return "f64";
        case ogss::KnownTypeID::ANY_REF:
            return "any";
        case ogss::KnownTypeID::STRING:
            return "string";

        default: {
            if (auto p = dynamic_cast<const ogss::AbstractPool *>(t)) {
                return *p->name;

            } else if (auto p = dynamic_cast<const ogss::internal::AbstractEnumPool *>(t)) {
                return *p->name;

            } else if (auto p = dynamic_cast<const ArrayType<ogss::api::Box> *>(t)) {
                return toString(p->base) += "[]";

            } else if (auto p = dynamic_cast<const ListType<ogss::api::Box> *>(t)) {
                return "list<" + toString(p->base) + ">";

            } else if (auto p = dynamic_cast<const SetType<ogss::api::Box> *>(t)) {
                return "set<" + toString(p->base) + ">";

            } else if (auto p = dynamic_cast<const MapType<ogss::api::Box, ogss::api::Box> *>(t)) {
                return "map<" + toString(p->keyType) + ", " + toString(p->valueType) + ">";

            } else {
                return "type:" + std::to_string(t->typeID);
            }
        }
    }
}