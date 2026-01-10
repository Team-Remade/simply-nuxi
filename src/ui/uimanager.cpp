#include "uimanager.h"
#include "viewportpanel.h"
#include "timelinepanel.h"
#include "scenetreepanel.h"
#include "propertiespanel.h"

UIManager::UIManager(wxWindow* parent)
{
    SetupLayout(parent);
}

UIManager::~UIManager()
{
    // Cleanup will be handled by wxWidgets parent-child relationships
}

void UIManager::SetupLayout(wxWindow* parent)
{
    // Create main horizontal splitter for left (Viewport/Timeline) and right (Scene Tree/Properties)
    mainSplitter = new wxSplitterWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D | wxSP_LIVE_UPDATE);
    mainSplitter->SetMinimumPaneSize(200);

    // Create left splitter for Viewport and Timeline
    leftSplitter = new wxSplitterWindow(mainSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D | wxSP_LIVE_UPDATE);
    leftSplitter->SetMinimumPaneSize(200);

    // Create right splitter for Scene Tree and Properties
    rightSplitter = new wxSplitterWindow(mainSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D | wxSP_LIVE_UPDATE);
    rightSplitter->SetMinimumPaneSize(100);

    // Create the panels
    viewportPanel = new ViewportPanel(leftSplitter);
    timelinePanel = new TimelinePanel(leftSplitter);
    sceneTreePanel = new SceneTreePanel(rightSplitter);
    propertiesPanel = new PropertiesPanel(rightSplitter);

    // Split left window vertically: Viewport on top, Timeline on bottom
    leftSplitter->SplitHorizontally(viewportPanel->GetWindow(), timelinePanel->GetWindow(), 400);

    // Split right window vertically: Scene Tree on top, Properties on bottom (70% height)
    rightSplitter->SplitHorizontally(sceneTreePanel->GetWindow(), propertiesPanel->GetWindow(), 300);

    // Split main window horizontally: left panels on left, right panels on right
    mainSplitter->SplitVertically(leftSplitter, rightSplitter, 640);
}

OpenGLCanvas* UIManager::GetViewport() const
{
    return viewportPanel ? viewportPanel->GetCanvas() : nullptr;
}

wxTreeCtrl* UIManager::GetSceneTree() const
{
    return sceneTreePanel ? sceneTreePanel->GetTreeCtrl() : nullptr;
}