#pragma once

#include <wx/wx.h>

class PropertiesPanel
{
public:
    PropertiesPanel(wxWindow* parent);
    ~PropertiesPanel();
    
    wxWindow* GetWindow() const { return panel; }
    
private:
    wxPanel* panel;
};