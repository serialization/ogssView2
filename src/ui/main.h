/*
 * main.h
 *
 *  Created on: Jul 5, 2020
 *      Author: Timm Felden
 */

#ifndef SRC_UI_MAIN_H_
#define SRC_UI_MAIN_H_

// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class MainApp : public wxApp {
public:
   bool OnInit() override;

   bool OnCmdLineParsed(wxCmdLineParser& parser) override;
};

class MainFrame : public wxFrame {
public:
   MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
private:
   void OnHello(wxCommandEvent &event);
   void OnExit(wxCommandEvent &event);
   void OnAbout(wxCommandEvent &event);

   wxDECLARE_EVENT_TABLE();
};

#endif /* SRC_UI_MAIN_H_ */
