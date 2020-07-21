//
// Created by feldentm on 20.07.20.
//

#ifndef OGSS_VIEW_PP_MAINFRAME_H
#define OGSS_VIEW_PP_MAINFRAME_H

// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP

#include <wx/wx.h>

#endif


enum {
    ID_Hello = 1
};


class MainFrame : public wxFrame {
public:
    MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    void OnHello(wxCommandEvent &event);

    void OnExit(wxCommandEvent &event);

    void OnAbout(wxCommandEvent &event);

    /**
     * Signal that a file has been loaded and that UI has to be updated.
     */
    void afterLoad();
    friend class MainApp;

wxDECLARE_EVENT_TABLE();
};

#include <wx/wxprec.h>
#include <wx/wx.h>

#endif //OGSS_VIEW_PP_MAINFRAME_H
