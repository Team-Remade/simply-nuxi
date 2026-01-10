#include "propertiespanel.h"
#include "darktheme.h"

PropertiesPanel::PropertiesPanel(wxWindow* parent)
{
    panel = new wxPanel(parent, wxID_ANY);
    panel->SetBackgroundColour(DarkTheme::PanelBackground);
    panel->SetForegroundColour(DarkTheme::Text);
    auto propertiesSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* propertiesLabel = new wxStaticText(panel, wxID_ANY, "Properties");
    propertiesLabel->SetForegroundColour(DarkTheme::Text);
    propertiesSizer->Add(propertiesLabel, 0, wxALL, 5);
    panel->SetSizer(propertiesSizer);
}

PropertiesPanel::~PropertiesPanel()
{
    // Cleanup handled by wxWidgets
}