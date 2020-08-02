//
// Created by feldentm on 01.08.20.
//

#include "ObjectPane.h"
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
  graph(new wxPanel(this)) {

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

    // panel layout
    {
        auto sizer = new wxBoxSizer(wxHORIZONTAL);
        SetSizer(sizer);

        sizer->Add(fields, 1, wxEXPAND | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL,
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
}

std::string ObjectPane::toString() { return Show::toString(ref); }
