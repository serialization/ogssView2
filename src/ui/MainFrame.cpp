//
// Created by feldentm on 20.07.20.
//

#include "main.h"
#include "MainFrame.h"

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
                EVT_MENU(ID_Hello, MainFrame::OnHello)
                EVT_MENU(wxID_EXIT, MainFrame::OnExit)
                EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString &title, const wxPoint &pos,
                     const wxSize &size) :
        wxFrame(NULL, wxID_ANY, title, pos, size) {

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
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
    wxMessageBox("This is a viewer for OGSS files.\nSee: ", "About OGSS View++",
                 wxOK | wxICON_INFORMATION);
}

void MainFrame::OnHello(wxCommandEvent &event) {
    wxLogMessage
        ("Hello world from wxWidgets!");
}

void MainFrame::afterLoad() {
    SetTitle("OGSS View++ - " + wxGetApp().get()->currentPath());
}
