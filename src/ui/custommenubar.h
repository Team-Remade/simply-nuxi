#pragma once

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/popupwin.h>
#include <wx/image.h>
#include <vector>
#include <functional>

// App icon embedded from bin2c generated header
extern const unsigned char appIcon01_png[];
extern const unsigned int appIcon01_png_size;

class CustomMenuBar : public wxPanel
{
public:
    CustomMenuBar(wxWindow* parent);
    
    // Add a menu with the given title
    void AddMenu(const wxString& title);
    
    // Add a menu item to the last added menu
    void AddMenuItem(const wxString& label, int id, const wxString& shortcut = wxEmptyString);
    
    // Add a separator to the last added menu
    void AddSeparator();
    
    // Bind an event handler to a menu item
    void BindMenuEvent(int id, std::function<void(wxCommandEvent&)> handler);
    
    // Set the application icon displayed in the title bar
    void SetAppIcon();
    
    // Set the application title displayed in the title bar area
    void SetAppTitle(const wxString& title) { appTitle = title; }
    
    // Add window control buttons (minimize, maximize, close)
    void AddWindowControls();
    
    // Bind window control events
    enum class ControlType { Minimize, Maximize, Close };
    void BindControlEvent(ControlType type, std::function<void(wxCommandEvent&)> handler);
    
    // Get the height of the menu bar for layout purposes
    int GetMenuBarHeight() const { return menuBarHeight; }
    
private:
    struct MenuItem
    {
        wxString label;
        int id;
        wxString shortcut;
        wxRect bounds;
        std::function<void(wxCommandEvent&)> handler;
    };
    
    struct Menu
    {
        wxString title;
        wxRect bounds;
        std::vector<MenuItem> items;
        bool isOpen = false;
    };
    
    enum class ControlButton { Minimize, Maximize, Restore, Close };
    struct WindowControl
    {
        ControlButton button;
        wxRect bounds;
        std::function<void(wxCommandEvent&)> handler;
        bool isHovered = false;
        bool isPressed = false;
    };
    
    std::vector<Menu> menus;
    std::vector<WindowControl> windowControls;
    int activeMenuIndex = -1;
    int menuBarHeight = 50;
    int titleBarHeight = 30;
    wxString appTitle;
    wxBitmap appIcon;
    bool isDragging = false;
    wxPoint dragStartPos;
    wxPoint frameStartPos;
    
    void OnPaint(wxPaintEvent& event);
    void OnMouseDown(wxMouseEvent& event);
    void OnMouseUp(wxMouseEvent& event);
    void OnMouseMotion(wxMouseEvent& event);
    void OnLeaveWindow(wxMouseEvent& event);
    void OnMenuItemClick(wxCommandEvent& event);
    
    void DrawMenuBar(wxDC& dc);
    void DrawTitleBar(wxDC& dc);
    void DrawWindowControls(wxDC& dc);
    void ShowMenuPopup(int menuIndex, const wxPoint& pos);
    
    wxWindow* parentFrame;
    
    class MenuPopup : public wxPopupWindow
    {
    public:
        MenuPopup(CustomMenuBar* parent, const Menu& menu);
        void OnPaint(wxPaintEvent& event);
        void OnMouseUp(wxMouseEvent& event);
        void OnMouseMotion(wxMouseEvent& event);
        void OnLeaveWindow(wxMouseEvent& event);
        
    private:
        CustomMenuBar* menuBar;
        Menu menu;
        int highlightedItem = -1;
        
        wxDECLARE_EVENT_TABLE();
    };
    
    MenuPopup* activePopup = nullptr;
    
    wxDECLARE_EVENT_TABLE();
};
