#include "scenetreepanel.h"
#include "darktheme.h"

SceneTreePanel::SceneTreePanel(wxWindow* parent)
{
    treeCtrl = new wxTreeCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT);
    treeCtrl->SetBackgroundColour(DarkTheme::PanelBackground);
    treeCtrl->SetForegroundColour(DarkTheme::Text);
    treeCtrl->AddRoot("Scene");
}

SceneTreePanel::~SceneTreePanel()
{
    // Cleanup handled by wxWidgets
}