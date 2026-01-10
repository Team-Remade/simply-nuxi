#include <wx/wx.h>
#include <stdio.h>

#include "shader.h"
#include "ui/uimanager.h"
#include "openglcanvas.h"

extern const char fragment[] = {
    #include "fragment.glsl.h"
};
extern const char vertex[] = {
    #include "vertex.glsl.h"
};

void error_callback(int error, const char* description)
{
    fprintf(stderr, "GL Error: %s\n", description);
}

class App : public wxApp
{
public:
    App(/* args */) {}
    bool OnInit() wxOVERRIDE;
};

class Frame : public wxFrame
{
public:
    Frame(const wxString &title);
private:
    UIManager* uiManager{nullptr};
};

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    Frame* frame = new Frame("Mine Imator Simply Remade: Nuxi");
    frame->Show(true);
    return true;
}

Frame::Frame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize)
{
    // Create MenuBar
    wxMenuBar *menuBar = new wxMenuBar();
    wxMenu *fileMenu = new wxMenu();
    fileMenu->Append(wxID_NEW, "New\tCtrl+N");
    fileMenu->Append(wxID_OPEN, "Open\tCtrl+O");
    fileMenu->Append(wxID_SAVE, "Save\tCtrl+S");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "Exit");
    menuBar->Append(fileMenu, "File");
    SetMenuBar(menuBar);

    // Create main sizer
    auto mainSizer = new wxBoxSizer(wxVERTICAL);

    // Initialize UI Manager
    uiManager = new UIManager(this);
    
    // Add UI to main sizer
    mainSizer->Add(uiManager->GetMainWindow(), 1, wxEXPAND | wxALL, FromDIP(5));

    SetSizerAndFit(mainSizer);

    // Bind events
    Bind(wxEVT_MENU, [this](wxCommandEvent &event) {
        switch (event.GetId()) {
            case wxID_EXIT:
                Close(true);
                break;
        }
    });
}


int main()
{
    wxApp::SetInstance(new App());
    wxEntryStart(0, nullptr);

    wxApp::GetInstance()->OnInit();

    

    wxApp::GetInstance()->OnRun();

    return 0;
}