#include "custommenubar.h"
#include "darktheme.h"

wxBEGIN_EVENT_TABLE(CustomMenuBar, wxPanel)
    EVT_PAINT(CustomMenuBar::OnPaint)
    EVT_LEFT_DOWN(CustomMenuBar::OnMouseDown)
    EVT_LEFT_UP(CustomMenuBar::OnMouseUp)
    EVT_MOTION(CustomMenuBar::OnMouseMotion)
    EVT_LEAVE_WINDOW(CustomMenuBar::OnLeaveWindow)
wxEND_EVENT_TABLE()

// MenuPopup implementation
wxBEGIN_EVENT_TABLE(CustomMenuBar::MenuPopup, wxPopupWindow)
    EVT_PAINT(CustomMenuBar::MenuPopup::OnPaint)
    EVT_LEFT_UP(CustomMenuBar::MenuPopup::OnMouseUp)
    EVT_MOTION(CustomMenuBar::MenuPopup::OnMouseMotion)
    EVT_LEAVE_WINDOW(CustomMenuBar::MenuPopup::OnLeaveWindow)
wxEND_EVENT_TABLE()

CustomMenuBar::MenuPopup::MenuPopup(CustomMenuBar* parent, const Menu& menuData)
    : wxPopupWindow(parent), menuBar(parent), menu(menuData)
{
    SetBackgroundColour(DarkTheme::PanelBackground);
    SetForegroundColour(DarkTheme::Text);
    
    // Calculate popup size
    int width = 200;
    int height = 0;
    
    for (const auto& item : menu.items)
    {
        if (item.id == wxID_SEPARATOR)
        {
            height += 10;
        }
        else
        {
            height += 30;
        }
    }
    
    SetSize(width, height);
}

void CustomMenuBar::MenuPopup::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    
    // Clear background
    dc.SetBackground(wxBrush(GetBackgroundColour()));
    dc.Clear();
    
    // Draw menu items
    dc.SetFont(GetFont());
    int y = 5;
    
    for (size_t i = 0; i < menu.items.size(); i++)
    {
        const auto& item = menu.items[i];
        
        if (item.id == wxID_SEPARATOR)
        {
            // Draw separator
            dc.SetPen(wxPen(DarkTheme::Accent, 1));
            dc.DrawLine(5, y + 5, GetSize().GetWidth() - 5, y + 5);
            y += 10;
        }
        else
        {
            wxRect itemBounds(5, y, GetSize().GetWidth() - 10, 25);
            
            // Highlight if this is the highlighted item
            if (static_cast<int>(i) == highlightedItem)
            {
                dc.SetBrush(wxBrush(DarkTheme::Accent));
                dc.SetPen(*wxTRANSPARENT_PEN);
                dc.DrawRectangle(itemBounds);
                dc.SetTextForeground(wxColour(255, 255, 255));
            }
            else
            {
                dc.SetTextForeground(DarkTheme::Text);
            }
            
            // Draw item text
            wxString displayText = item.label;
            if (!item.shortcut.IsEmpty())
            {
                displayText += wxT("\t") + item.shortcut;
            }
            
            dc.DrawText(displayText, itemBounds.GetLeft() + 5, itemBounds.GetTop() + 5);
            
            y += 30;
        }
    }
}

void CustomMenuBar::MenuPopup::OnMouseUp(wxMouseEvent& event)
{
    wxPoint pos = event.GetPosition();
    int y = 5;
    
    for (size_t i = 0; i < menu.items.size(); i++)
    {
        const auto& item = menu.items[i];
        
        if (item.id == wxID_SEPARATOR)
        {
            y += 10;
            continue;
        }
        
        wxRect itemBounds(5, y, GetSize().GetWidth() - 10, 25);
        
        if (itemBounds.Contains(pos) && item.handler)
        {
            wxCommandEvent evt(wxEVT_MENU, item.id);
            item.handler(evt);
            
            // Close the popup
            menuBar->activePopup = nullptr;
            Hide();
            return;
        }
        
        y += 30;
    }
    
    // Close the popup if clicking outside items
    menuBar->activePopup = nullptr;
    Hide();
}

void CustomMenuBar::MenuPopup::OnMouseMotion(wxMouseEvent& event)
{
    wxPoint pos = event.GetPosition();
    int y = 5;
    highlightedItem = -1;
    
    for (size_t i = 0; i < menu.items.size(); i++)
    {
        const auto& item = menu.items[i];
        
        if (item.id == wxID_SEPARATOR)
        {
            y += 10;
            continue;
        }
        
        wxRect itemBounds(5, y, GetSize().GetWidth() - 10, 25);
        
        if (itemBounds.Contains(pos))
        {
            highlightedItem = static_cast<int>(i);
            Refresh();
            return;
        }
        
        y += 30;
    }
    
    highlightedItem = -1;
    Refresh();
}

void CustomMenuBar::MenuPopup::OnLeaveWindow(wxMouseEvent& event)
{
    // Check if mouse is entering the menu bar
    wxPoint mousePos = wxGetMousePosition();
    wxPoint menuBarPos = menuBar->GetScreenPosition();
    wxSize menuBarSize = menuBar->GetSize();
    wxRect menuBarRect(menuBarPos, menuBarSize);
    
    // Only close if mouse is not entering the menu bar
    if (!menuBarRect.Contains(mousePos))
    {
        menuBar->activePopup = nullptr;
        Hide();
    }
}

void CustomMenuBar::ShowMenuPopup(int menuIndex, const wxPoint& pos)
{
    // Close any existing popup
    if (activePopup)
    {
        activePopup->Hide();
        activePopup = nullptr;
    }
    
    // Show new popup
    activePopup = new MenuPopup(this, menus[menuIndex]);
    activePopup->Position(pos, wxSize(0, 0));
    activePopup->Show();
}

CustomMenuBar::CustomMenuBar(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(-1, menuBarHeight))
{
    SetBackgroundColour(DarkTheme::Background);
    SetForegroundColour(DarkTheme::Text);
    
    // Set a minimum size for the menu bar
    SetMinSize(wxSize(-1, menuBarHeight));
}

void CustomMenuBar::AddMenu(const wxString& title)
{
    Menu menu;
    menu.title = title;
    menus.push_back(menu);
}

void CustomMenuBar::AddMenuItem(const wxString& label, int id, const wxString& shortcut)
{
    if (menus.empty()) return;
    
    MenuItem item;
    item.label = label;
    item.id = id;
    item.shortcut = shortcut;
    menus.back().items.push_back(item);
}

void CustomMenuBar::AddSeparator()
{
    if (menus.empty()) return;
    
    // Add a special separator item
    MenuItem separator;
    separator.label = wxT("-");
    separator.id = wxID_SEPARATOR;
    menus.back().items.push_back(separator);
}

void CustomMenuBar::BindMenuEvent(int id, std::function<void(wxCommandEvent&)> handler)
{
    for (auto& menu : menus)
    {
        for (auto& item : menu.items)
        {
            if (item.id == id)
            {
                item.handler = handler;
                return;
            }
        }
    }
}

void CustomMenuBar::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    
    // Clear the background
    dc.SetBackground(wxBrush(GetBackgroundColour()));
    dc.Clear();
    
    // Draw the menu bar
    DrawMenuBar(dc);
    
    // Menu dropdowns are handled by popup windows, no need to draw here
}

void CustomMenuBar::DrawMenuBar(wxDC& dc)
{
    dc.SetFont(GetFont());
    dc.SetTextForeground(DarkTheme::Text);
    
    int x = 5;
    int y = 0;
    int height = menuBarHeight;
    
    for (size_t i = 0; i < menus.size(); i++)
    {
        wxString title = menus[i].title;
        wxSize textSize = dc.GetTextExtent(title);
        
        // Store the bounds for hit testing
        menus[i].bounds = wxRect(x, y, textSize.GetWidth() + 20, height);
        
        // Draw menu title
        dc.DrawText(title, x + 10, (height - textSize.GetHeight()) / 2);
        
        x += textSize.GetWidth() + 25;
    }
}

// DrawMenu function is no longer needed since we use popup windows

// DrawMenuItem function removed - drawing is now handled directly in DrawMenu

void CustomMenuBar::OnMouseDown(wxMouseEvent& event)
{
    wxPoint pos = event.GetPosition();
    
    // Close any existing popup
    if (activePopup)
    {
        activePopup->Hide();
        activePopup = nullptr;
    }
    
    // Check if a menu was clicked
    for (size_t i = 0; i < menus.size(); i++)
    {
        if (menus[i].bounds.Contains(pos))
        {
            // Show popup menu at the bottom of the menu item
            wxPoint menuPos = menus[i].bounds.GetBottomLeft();
            menuPos = ClientToScreen(menuPos);
            
            ShowMenuPopup(static_cast<int>(i), menuPos);
            return;
        }
    }
}

void CustomMenuBar::OnMouseUp(wxMouseEvent& event)
{
    // Mouse up is handled by the popup window
}

void CustomMenuBar::OnMouseMotion(wxMouseEvent& event)
{
    // Mouse motion is handled by the popup window
}

void CustomMenuBar::OnLeaveWindow(wxMouseEvent& event)
{
    // Don't close the popup immediately when leaving the menu bar
    // The popup will handle its own closing logic when mouse leaves it
}