#include "darksplitter.h"
#include <wx/graphics.h>

void DarkSplitter::DrawSash(wxDC& dc)
{
    int sashPos = GetSashPosition();
    wxSplitMode splitMode = GetSplitMode();
    
    wxRect sashRect;
    if (splitMode == wxSPLIT_VERTICAL)
    {
        sashRect = wxRect(sashPos, 0, 4, GetSize().GetHeight());
    }
    else // wxSPLIT_HORIZONTAL
    {
        sashRect = wxRect(0, sashPos, GetSize().GetWidth(), 4);
    }
    
    // Draw sash with dark theme background
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(wxBrush(DarkTheme::Sash));
    dc.DrawRectangle(sashRect);
}
