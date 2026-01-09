#include <wx/wx.h>
#include <GL/glew.h>
#include <wx/glcanvas.h>
#include <stdio.h>

#include "shader.h"

extern const char fragment[] = {
    #include "fragment.glsl.h"
};
extern const char vertex[] = {
    #include "vertex.glsl.h"
};

void error_callback(int error, const char* description)
{
    fprintf(stderr, "GL Error: %s\n", description);
}

class App : public wxApp
{
public:
    App(/* args */) {}
    bool OnInit() wxOVERRIDE;  
};

class OpenGLCanvas;

class Frame : public wxFrame
{
private:
    OpenGLCanvas* canvas{nullptr};
public:
    Frame(const wxString& title);
};

class OpenGLCanvas : public wxGLCanvas
{
public:
    OpenGLCanvas(Frame *parent, const wxGLAttributes& canvasAttrs);
    ~OpenGLCanvas();

    bool InitGLFuctions();
    bool InitGL();

    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);

private:
    wxGLContext* openGLContext{nullptr};
    bool isInitialized{false};

    unsigned int VAO, VBO;
    Shader shader = Shader(vertex, fragment);
};

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    Frame* frame = new Frame("Mine Imator Simply Remade: Nuxi");
    frame->Show(true);
    return true;
}

Frame::Frame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(640, 480))
{
    wxGLAttributes canvasAttrs;
    canvasAttrs.PlatformDefaults().Defaults().EndList();

    canvas = new OpenGLCanvas(this, canvasAttrs);
    canvas->SetMinSize(FromDIP(wxSize(640, 480)));
}

OpenGLCanvas::OpenGLCanvas(Frame *parent, const wxGLAttributes& canvasAttrs) : wxGLCanvas(parent, canvasAttrs)
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
        return;

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
        if (!InitGL())
            return;

        isInitialized = true;
    }

    if (!isInitialized)
        return;
    
    wxSize size = event.GetSize() * GetContentScaleFactor();
    int w, h;
    GetClientSize(&w, &h);

    glViewport(0, 0, w, h);

    event.Skip();
}