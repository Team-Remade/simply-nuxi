#include "openglcanvas.h"

extern const char fragment[];
extern const char vertex[];

OpenGLCanvas::OpenGLCanvas(wxWindow *parent, const wxGLAttributes &canvasAttrs) : wxGLCanvas(parent, canvasAttrs)
{
    wxGLContextAttrs contextAttrs;
    contextAttrs.PlatformDefaults().CoreProfile().OGLVersion(4, 1).EndList();
    openGLContext = new wxGLContext(this, nullptr, &contextAttrs);

    if (!openGLContext->IsOK())
    {
        wxMessageBox("A compatible driver was not found. You need OpenGL 4.1 or higher.", "OpenGL version error", wxOK | wxICON_ERROR, this);
        delete openGLContext;
        openGLContext = nullptr;
    }
    
    Bind(wxEVT_PAINT, &OpenGLCanvas::OnPaint, this);
    Bind(wxEVT_SIZE, &OpenGLCanvas::OnSize, this);
}

OpenGLCanvas::~OpenGLCanvas()
{
    delete openGLContext;
}

bool OpenGLCanvas::InitGLFuctions()
{
    if (!isInitialized)
    {
        if (!openGLContext->SetCurrent(*this))
            return false;

        if (glewInit() != GLEW_OK)
            return false;

        isInitialized = true;
    }

    return true;
}

bool OpenGLCanvas::InitGL()
{
    if (!InitGLFuctions())
        return false;
    
    SetCurrent(*openGLContext);

    shader = Shader(vertex, fragment);

    float points[6] = {
        // left
        -0.8f, -0.5f,

        // top
        0.0f, 0.5f,

        // right
        0.8f, -0.5f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}

void OpenGLCanvas::OnPaint(wxPaintEvent& event)
{
    if (!isInitialized)
    {
        if (!InitGL())
        {
            return;
        }
        isInitialized = true;
    }

    wxPaintDC(this);

    if (!openGLContext->SetCurrent(*this))
        return;

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glFlush();
    SwapBuffers();
}

void OpenGLCanvas::OnSize(wxSizeEvent& event)
{
    bool firstAppearance = IsShownOnScreen() && !isInitialized;

    if (firstAppearance)
    {
        return;
        if (!InitGL())
            return;

        isInitialized = true;
    }

    if (!isInitialized)
        return;
    
    wxSize size = event.GetSize() * GetContentScaleFactor();
    int w, h;
    GetClientSize(&w, &h);

    if (w > 0 && h > 0) {
        glViewport(0, 0, w, h);
        Refresh(); // Force a repaint after viewport change
    }

    event.Skip();
}