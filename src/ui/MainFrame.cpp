//
// Created by feldentm on 20.07.20.
//

#include "MainFrame.h"
#include "TypePane.h"
#include "main.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
  EVT_MENU(ID_OpenFile, MainFrame::OnOpenFile)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
      EVT_MENU(wxID_ABOUT, MainFrame::OnAbout) wxEND_EVENT_TABLE()

        MainFrame::MainFrame(const wxString &title, const wxPoint &pos,
                             const wxSize &size) :
  wxFrame(NULL, wxID_ANY, title, pos, size),
  body(new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                         wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE |
                           wxAUI_NB_TAB_EXTERNAL_MOVE |
                           wxAUI_NB_WINDOWLIST_BUTTON |
                           wxAUI_NB_CLOSE_ON_ACTIVE_TAB | wxAUI_NB_TOP)),
  types(new TypePane(body)) {

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_OpenFile, "&Open...\tCtrl-O",
                     "Open a binary graph in this window.");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    wxFrameBase::SetMenuBar(menuBar);
    wxFrameBase::CreateStatusBar();
    wxFrameBase::SetStatusText("Welcome!");

    SetSizer(new wxBoxSizer(wxHORIZONTAL));
    GetSizer()->Add(body, 1, wxEXPAND | wxALL, 0);

    wxTopLevelWindowBase::SetMinSize(wxSize(640, 480));

    // choose a less horrible tab presentation
    body->SetArtProvider(new wxAuiSimpleTabArt());

    // events
    body->Bind(wxEVT_AUINOTEBOOK_PAGE_CHANGED, &MainFrame::onTabChange, this);
    body->Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSE, &MainFrame::onTabClose, this);
}

void MainFrame::OnExit(wxCommandEvent &event) { wxWindowBase::Close(true); }

void MainFrame::OnAbout(wxCommandEvent &event) {
    wxMessageBox("This is a viewer for binary OGSS graphs.\n"
                 "https://github.com/serialization/ogssView2\n"
                 "Timm Felden\n"
                 "Apache-2.0 License",
                 "About OGSS View++", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnOpenFile(wxCommandEvent &event) {
    wxFileDialog openFileDialog(this, _("Open a binary OGSS graph"), "", "",
                                wxFileSelectorDefaultWildcardStr,
                                wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return; // the user changed idea...

    if (!wxGetApp().load(openFileDialog.GetPath().ToStdString())) {
        wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
        return;
    }
}

void MainFrame::afterLoad() {
    SetTitle("OGSS View++ - " + wxGetApp().get()->currentPath());
    types->afterLoad();
}

void MainFrame::onTabChange(wxAuiNotebookEvent &event) {
    auto tab = dynamic_cast<TabEntry *>(body->GetCurrentPage());
    SetStatusText(tab->toString());
}

void MainFrame::onTabClose(wxAuiNotebookEvent &event) {
    // type pane cannot be closed, because it cannot be reopened
    if (dynamic_cast<TypePane *>(body->GetPage(event.GetSelection()))) {
        event.Veto();
        wxFrameBase::SetStatusText("Types cannot be closed.");
    }
}
