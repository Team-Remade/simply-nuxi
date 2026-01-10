#include "timelinepanel.h"
#include "darktheme.h"

TimelinePanel::TimelinePanel(wxWindow* parent)
{
    panel = new wxPanel(parent, wxID_ANY);
    panel->SetBackgroundColour(DarkTheme::PanelBackground);
    panel->SetForegroundColour(DarkTheme::Text);
    auto timelineSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* timelineLabel = new wxStaticText(panel, wxID_ANY, "Timeline");
    timelineLabel->SetForegroundColour(DarkTheme::Text);
    timelineSizer->Add(timelineLabel, 0, wxALL, 5);
    panel->SetSizer(timelineSizer);
}

TimelinePanel::~TimelinePanel()
{
    // Cleanup handled by wxWidgets
}