#include "propertiespanel.h"

PropertiesPanel::PropertiesPanel(wxWindow* parent)
{
    panel = new wxPanel(parent, wxID_ANY);
    panel->SetBackgroundColour(wxColour(40, 40, 40));
    auto propertiesSizer = new wxBoxSizer(wxVERTICAL);
    propertiesSizer->Add(new wxStaticText(panel, wxID_ANY, "Properties"), 0, wxALL, 5);
    panel->SetSizer(propertiesSizer);
}

PropertiesPanel::~PropertiesPanel()
{
    // Cleanup handled by wxWidgets
}