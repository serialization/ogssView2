#include "main.h"
#include "MainFrame.h"
#include "../empty/File.h"

#include <wx/cmdline.h>

wxIMPLEMENT_APP(MainApp);

using empty::api::File;

bool MainApp::OnInit() {
    // call default behaviour (mandatory)
    if (!wxApp::OnInit())
        return false;

    MainFrame *frame = new MainFrame("OGSS View++", wxPoint(50, 50),
                                     wxSize(1280, 800));
    frame->Show(true);
    return true;
}


static const wxCmdLineEntryDesc g_cmdLineDesc[] =
        {
                {wxCMD_LINE_SWITCH, "h", "help",
                        "Print this help",
                        wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP},
                {wxCMD_LINE_NONE}
        };

void MainApp::OnInitCmdLine(wxCmdLineParser &parser) {
    parser.SetDesc(g_cmdLineDesc);
    // must refuse '/' as parameter starter or cannot use "/path" style paths
    parser.SetSwitchChars(wxT("-"));

    parser.AddUsageText("Pass one path to a binary OGSS file to display its contents.");
    parser.AddParam("path", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
}

bool MainApp::OnCmdLineParsed(wxCmdLineParser &parser) {
    // the parser allows one or zero parameters which are paths
    if (0 != parser.GetParamCount()) {
        auto sg = File::open(parser.GetParam(0).ToStdString(), ogss::api::read | ogss::api::readOnly);
        std::cout <<
                  sg->size() << std::endl;
        
        delete sg;
    }
    return true;
}

