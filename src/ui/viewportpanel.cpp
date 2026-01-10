#include "../openglcanvas.h"
#include "viewportpanel.h"

ViewportPanel::ViewportPanel(wxWindow* parent)
{
    // Create OpenGL Canvas
    wxGLAttributes canvasAttrs;
    canvasAttrs.PlatformDefaults().Defaults().EndList();
    canvas = new OpenGLCanvas(parent, canvasAttrs);
    canvas->SetMinSize(wxSize(640, 480));
}

ViewportPanel::~ViewportPanel()
{
    // Cleanup handled by wxWidgets
}