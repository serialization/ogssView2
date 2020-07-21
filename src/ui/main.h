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

#include "../empty/File.h"
#include "MainFrame.h"

class MainApp : public wxApp {
    empty::api::File *graph;
    MainFrame *frame;
public:
    MainApp();

    ~MainApp() override;

    /**
     * Try to load the argument path
     * @return true iff the file could be loaded
     */
    bool load(const std::string &path);

    /**
     * @return the current graph. It is never null after a successful load operation.
     */
    const empty::api::File *get() const {
        return graph;
    }

    bool OnInit() override;

    void OnInitCmdLine(wxCmdLineParser &parser) override;

    bool OnCmdLineParsed(wxCmdLineParser &parser) override;
};

DECLARE_APP(MainApp)

#endif /* SRC_UI_MAIN_H_ */
