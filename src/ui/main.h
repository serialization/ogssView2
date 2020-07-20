/*
 * main.h
 *
 *  Created on: Jul 5, 2020
 *      Author: Timm Felden
 */

#ifndef SRC_UI_MAIN_H_
#define SRC_UI_MAIN_H_

#include <wx/wxprec.h>
#include <wx/cmdline.h>

#ifndef WX_PRECOMP

#include <wx/wx.h>

#endif

class MainApp : public wxApp {
public:
    bool OnInit() override;

    void OnInitCmdLine(wxCmdLineParser &parser) override;

    bool OnCmdLineParsed(wxCmdLineParser &parser) override;
};

DECLARE_APP(MainApp)

#endif /* SRC_UI_MAIN_H_ */
