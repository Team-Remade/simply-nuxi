#pragma once

#include <wx/splitter.h>
#include "darktheme.h"

class DarkSplitter : public wxSplitterWindow
{
public:
    DarkSplitter(wxWindow* parent, wxWindowID id = wxID_ANY, 
                 const wxPoint& point = wxDefaultPosition, 
                 const wxSize& size = wxDefaultSize, 
                 long style = wxSP_3D, 
                 const wxString& name = "splitter")
        : wxSplitterWindow(parent, id, point, size, style, name)
    {
        SetBackgroundColour(DarkTheme::Background);
    }

protected:
    virtual void DrawSash(wxDC& dc) override;
};
