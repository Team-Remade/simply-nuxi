#pragma once

#include <wx/wx.h>
#include <wx/treectrl.h>

class SceneTreePanel
{
public:
    SceneTreePanel(wxWindow* parent);
    ~SceneTreePanel();
    
    wxWindow* GetWindow() const { return treeCtrl; }
    wxTreeCtrl* GetTreeCtrl() const { return treeCtrl; }
    
private:
    wxTreeCtrl* treeCtrl;
};