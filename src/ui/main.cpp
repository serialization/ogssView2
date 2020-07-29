#include "main.h"

#include <wx/cmdline.h>

wxIMPLEMENT_APP(MainApp);

using empty::api::File;

bool MainApp::OnInit() {
    // call default behaviour (mandatory)
    if (!wxApp::OnInit())
        return false;

    frame = new MainFrame("OGSS View++", wxPoint(50, 50),
                          wxSize(1280, 800));
    // note: load happens before init. Hence we have to signal it here
    if (nullptr != graph)
        frame->afterLoad();

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
        return load(parser.GetParam(0).ToStdString());
    }
    return true;
}

MainApp::MainApp() : wxApp(), graph(nullptr), frame(nullptr) {
}

MainApp::~MainApp() {
    delete graph;
}

bool MainApp::load(const std::string &path) {
    // load graph and overwrite current graph on success
    empty::api::File *next;
    try {
        next = File::open(path, ogss::api::read | ogss::api::readOnly);
    } catch (ogss::Exception &e) {
        std::cerr << "Failed to load " << path << std::endl
                  << e.what() << std::endl;
        return false;
    }

    if (nullptr != graph) {
        delete graph;
    }
    graph = next;

    // TODO delete all open object views

    if (nullptr != frame)
        frame->afterLoad();

    return true;
}

