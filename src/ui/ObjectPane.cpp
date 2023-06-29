//
// Created by feldentm on 01.08.20.
//

#include "ObjectPane.h"
#include "GraphExport.h"
#include "Show.h"
#include "main.h"
#include <ogss/iterators/FieldIterator.h>

ObjectPane::ObjectPane(wxAuiNotebook *parent,
                       const ogss::internal::UnknownObject *const ref) :
  TabEntry(parent),
  ref(ref),
  fields(new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
                        wxTE_MULTILINE | wxTE_READONLY)),

  referrals(new wxPanel(this)),

  graph(new wxPanel(this)),
  maxDistance(new wxSlider(graph, wxID_ANY, 10, 1, 100)),
  maxNodes(new wxSlider(graph, wxID_ANY, 30, 1, 100)),
  maxContainerItems(new wxSlider(graph, wxID_ANY, 10, 1, 100)),
  showPDF(new wxButton(graph, wxID_ANY, "Show as PDF", wxDefaultPosition,
                       wxDefaultSize, wxBU_EXACTFIT)),
  showXDOT(new wxButton(graph, wxID_ANY, "Show in xdot", wxDefaultPosition,
                        wxDefaultSize, wxBU_EXACTFIT)) {

    const auto sg = wxGetApp().get();

    // calculate fields contents
    {
        fields->AppendText(Show::toString(ref));

        fields->AppendText(" {\n");

        auto t = sg->pool(ref);

        auto fs = t->allFields();
        while (fs.hasNext()) {
            auto f = fs.next();

            fields->AppendText("\n   ");
            fields->AppendText(Show::toString(f->type));
            fields->AppendText(" ");
            fields->AppendText(*f->name);

            fields->AppendText(" = ");

            fields->AppendText(Show::toString(f->type, f->getR(ref)));
            fields->AppendText("\n");
        }

        fields->AppendText("\n}");
    }

    // referrals
    { new wxStaticText(referrals, wxID_ANY, "TODO referrals"); }

    // panel layout
    {
        auto sizer = new wxBoxSizer(wxVERTICAL);
        graph->SetSizer(sizer);

        sizer->Add(maxDistance, 1,
                   wxEXPAND | wxALIGN_TOP | wxALIGN_CENTER_HORIZONTAL, 0);
        sizer->Add(maxContainerItems, 1,
                   wxEXPAND | wxALIGN_TOP | wxALIGN_CENTER_HORIZONTAL, 0);
        sizer->Add(maxNodes, 1,
                   wxEXPAND | wxALIGN_TOP | wxALIGN_CENTER_HORIZONTAL, 0);
        sizer->Add(showPDF, 1,
                   wxEXPAND | wxALIGN_TOP | wxALIGN_CENTER_HORIZONTAL, 0);
        sizer->Add(showXDOT, 1,
                   wxEXPAND | wxALIGN_TOP | wxALIGN_CENTER_HORIZONTAL, 0);
    }
    {
        auto sizer = new wxBoxSizer(wxHORIZONTAL);
        SetSizer(sizer);

        sizer->Add(fields, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL,
                   0);
        sizer->Add(referrals, 1,
                   wxEXPAND | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 0);
        sizer->Add(graph, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL,
                   0);
    }

    // for some reason type has by default the wrong style
    {
        const auto base =
          wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT);
        fields->SetBackgroundColour(
          wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
        fields->SetForegroundColour(base);
    }

    // bind events
    showXDOT->Bind(wxEVT_BUTTON, &ObjectPane::onShowXDOT, this);
}

std::string ObjectPane::toString() { return Show::toString(ref); }

void ObjectPane::onShowXDOT(wxCommandEvent &event) {
    GraphExport g(toString(), maxDistance->GetValue(),
                  maxContainerItems->GetValue(), maxNodes->GetValue(),
                  ref->pool, ogss::api::box(ref));

    g.openXDOT();
}
