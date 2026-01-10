#include "timelinepanel.h"

TimelinePanel::TimelinePanel(wxWindow* parent)
{
    panel = new wxPanel(parent, wxID_ANY);
    panel->SetBackgroundColour(wxColour(50, 50, 50));
    auto timelineSizer = new wxBoxSizer(wxVERTICAL);
    timelineSizer->Add(new wxStaticText(panel, wxID_ANY, "Timeline"), 0, wxALL, 5);
    panel->SetSizer(timelineSizer);
}

TimelinePanel::~TimelinePanel()
{
    // Cleanup handled by wxWidgets
}