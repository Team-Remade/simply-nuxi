#include <wx/wx.h>
#include <stdio.h>

#include "shader.h"
#include "ui/uimanager.h"
#include "openglcanvas.h"
#include "ui/darktheme.h"
#include "ui/custommenubar.h"

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
    // Apply dark theme to the frame
    SetBackgroundColour(DarkTheme::Background);
    SetForegroundColour(DarkTheme::Text);

    // Create custom menu bar
    CustomMenuBar* customMenuBar = new CustomMenuBar(this);
    
    // Add File menu
    customMenuBar->AddMenu("File");
    customMenuBar->AddMenuItem("New", wxID_NEW, "Ctrl+N");
    customMenuBar->AddMenuItem("Open", wxID_OPEN, "Ctrl+O");
    customMenuBar->AddMenuItem("Save", wxID_SAVE, "Ctrl+S");
    customMenuBar->AddSeparator();
    customMenuBar->AddMenuItem("Exit", wxID_EXIT);

    // Create main sizer
    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    
    // Add custom menu bar to sizer
    mainSizer->Add(customMenuBar, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, FromDIP(5));
    
    // Initialize UI Manager
    uiManager = new UIManager(this);
      
    // Add UI to main sizer
    mainSizer->Add(uiManager->GetMainWindow(), 1, wxEXPAND | wxALL, FromDIP(5));

    SetSizerAndFit(mainSizer);

    // Bind menu events to custom menu bar
    customMenuBar->BindMenuEvent(wxID_EXIT, [this](wxCommandEvent &event) {
        Close(true);
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