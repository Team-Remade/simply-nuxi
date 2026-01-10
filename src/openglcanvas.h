#pragma once

#include <wx/wx.h>
#include <GL/glew.h>
#include <wx/glcanvas.h>

#include "shader.h"

class OpenGLCanvas : public wxGLCanvas
{
public:
    OpenGLCanvas(wxWindow *parent, const wxGLAttributes &canvasAttrs);
    ~OpenGLCanvas();

    bool InitGLFuctions();
    bool InitGL();

    void OnPaint(wxPaintEvent &event);
    void OnSize(wxSizeEvent &event);

private:
    wxGLContext *openGLContext;
    bool isInitialized{false};

    unsigned int VAO, VBO;
    Shader shader;
};