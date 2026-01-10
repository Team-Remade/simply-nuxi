#pragma once

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <wx/treectrl.h>
#include "darksplitter.h"

class OpenGLCanvas;
class ViewportPanel;
class TimelinePanel;
class SceneTreePanel;
class PropertiesPanel;

class UIManager
{
public:
    UIManager(wxWindow* parent);
    ~UIManager();
    
    wxWindow* GetMainWindow() const { return mainSplitter; }
    OpenGLCanvas* GetViewport() const;
    wxTreeCtrl* GetSceneTree() const;
    
private:
    DarkSplitter* mainSplitter;
    DarkSplitter* leftSplitter;
    DarkSplitter* rightSplitter;
    
    ViewportPanel* viewportPanel;
    TimelinePanel* timelinePanel;
    SceneTreePanel* sceneTreePanel;
    PropertiesPanel* propertiesPanel;
    
    void SetupLayout(wxWindow* parent);
};