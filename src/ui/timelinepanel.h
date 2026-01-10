#pragma once

#include <wx/wx.h>

class TimelinePanel
{
public:
    TimelinePanel(wxWindow* parent);
    ~TimelinePanel();
    
    wxWindow* GetWindow() const { return panel; }
    
private:
    wxPanel* panel;
};