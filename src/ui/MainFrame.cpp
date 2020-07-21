//
// Created by feldentm on 20.07.20.
//

#include "main.h"
#include "MainFrame.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
                EVT_MENU(ID_OpenFile, MainFrame::OnOpenFile)
                EVT_MENU(wxID_EXIT, MainFrame::OnExit)
                EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString &title, const wxPoint &pos,
                     const wxSize &size) :
        wxFrame(NULL, wxID_ANY, title, pos, size) {

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
    wxFrameBase::SetStatusText("Welcome to wxWidgets!");
}

void MainFrame::OnExit(wxCommandEvent &event) {
    wxWindowBase::Close(true);
}

void MainFrame::OnAbout(wxCommandEvent &event) {
    wxMessageBox("This is a viewer for binary OGSS graphs.\nhttps://github.com/serialization/ogssView2\nTimm Felden\nApache-2.0 License", "About OGSS View++",
                 wxOK | wxICON_INFORMATION);
}

void MainFrame::OnOpenFile(wxCommandEvent &event) {
    wxFileDialog
            openFileDialog(this, _("Open a binary OGSS graph"), "", "",
                           wxFileSelectorDefaultWildcardStr, wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed idea...

    if (!wxGetApp().load(openFileDialog.GetPath().ToStdString())) {
        wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
        return;
    }
}

void MainFrame::afterLoad() {
    SetTitle("OGSS View++ - " + wxGetApp().get()->currentPath());
}
