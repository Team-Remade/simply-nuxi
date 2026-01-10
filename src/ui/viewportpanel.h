#pragma once

#include <wx/wx.h>
#include <wx/glcanvas.h>

class OpenGLCanvas;

class ViewportPanel
{
public:
    ViewportPanel(wxWindow* parent);
    ~ViewportPanel();
    
    wxWindow* GetWindow() const { return reinterpret_cast<wxWindow*>(canvas); }
    OpenGLCanvas* GetCanvas() const { return canvas; }
    
private:
    OpenGLCanvas* canvas;
};