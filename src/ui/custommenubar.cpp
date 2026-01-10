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

CustomMenuBar::CustomMenuBar(wxWindow* parent) 
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(-1, 50)), parentFrame(parent)
{
    SetBackgroundColour(DarkTheme::Background);
    SetForegroundColour(DarkTheme::Text);
    
    // Set a minimum size for the menu bar
    SetMinSize(wxSize(-1, 50));
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

void CustomMenuBar::AddWindowControls()
{
    // Add minimize, maximize/restore, and close buttons
    windowControls.clear();
    
    WindowControl minimize;
    minimize.button = ControlButton::Minimize;
    minimize.handler = nullptr;
    windowControls.push_back(minimize);
    
    WindowControl maximize;
    maximize.button = ControlButton::Maximize;
    maximize.handler = nullptr;
    windowControls.push_back(maximize);
    
    WindowControl close;
    close.button = ControlButton::Close;
    close.handler = nullptr;
    windowControls.push_back(close);
}

void CustomMenuBar::BindControlEvent(ControlType type, std::function<void(wxCommandEvent&)> handler)
{
    for (auto& control : windowControls)
    {
        if ((type == ControlType::Minimize && control.button == ControlButton::Minimize) ||
            (type == ControlType::Maximize && (control.button == ControlButton::Maximize || control.button == ControlButton::Restore)) ||
            (type == ControlType::Close && control.button == ControlButton::Close))
        {
            control.handler = handler;
            return;
        }
    }
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
    
    // Draw the title bar
    DrawTitleBar(dc);
    
    // Draw the menu bar
    DrawMenuBar(dc);
    
    // Draw window controls
    DrawWindowControls(dc);
    
    // Menu dropdowns are handled by popup windows, no need to draw here
}

void CustomMenuBar::DrawTitleBar(wxDC& dc)
{
    int width, height;
    GetSize(&width, &height);
    
    // Draw title bar background
    dc.SetBrush(wxBrush(DarkTheme::PanelBackground));
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawRectangle(0, 0, width, titleBarHeight);
    
    // Draw title text
    if (!appTitle.IsEmpty())
    {
        dc.SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        dc.SetTextForeground(DarkTheme::Text);
        wxSize textSize = dc.GetTextExtent(appTitle);
        dc.DrawText(appTitle, 10, (titleBarHeight - textSize.GetHeight()) / 2);
    }
}

void CustomMenuBar::DrawMenuBar(wxDC& dc)
{
    int width, height;
    GetSize(&width, &height);
    
    dc.SetFont(GetFont());
    dc.SetTextForeground(DarkTheme::Text);
    
    int x = 10;
    int y = titleBarHeight;
    int menuHeight = menuBarHeight - titleBarHeight;
    
    // Draw menu bar background (below title bar)
    dc.SetBrush(wxBrush(DarkTheme::Background));
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawRectangle(0, titleBarHeight, width, menuHeight);
    
    for (size_t i = 0; i < menus.size(); i++)
    {
        wxString title = menus[i].title;
        wxSize textSize = dc.GetTextExtent(title);
        
        // Store the bounds for hit testing
        menus[i].bounds = wxRect(x, y, textSize.GetWidth() + 20, menuHeight);
        
        // Highlight if menu is open
        if (menus[i].isOpen)
        {
            dc.SetBrush(wxBrush(DarkTheme::Accent));
            dc.DrawRectangle(menus[i].bounds);
        }
        
        // Draw menu title
        dc.DrawText(title, x + 10, y + (menuHeight - textSize.GetHeight()) / 2);
        
        x += textSize.GetWidth() + 25;
    }
}

void CustomMenuBar::DrawWindowControls(wxDC& dc)
{
    int width, height;
    GetSize(&width, &height);
    
    int buttonSize = 30;
    int buttonY = 5;
    int x = width - 10;
    
    // Draw controls in reverse order (close on the right)
    for (auto it = windowControls.rbegin(); it != windowControls.rend(); ++it)
    {
        x -= buttonSize;
        it->bounds = wxRect(x, buttonY, buttonSize, buttonSize);
        
        // Determine which button type to show
        ControlButton actualButton = it->button;
        wxTopLevelWindow* tlw = wxDynamicCast(parentFrame, wxTopLevelWindow);
        if (actualButton == ControlButton::Maximize && tlw && tlw->IsMaximized())
        {
            actualButton = ControlButton::Restore;
        }
        
        // Draw button background
        wxColour bgColor = DarkTheme::Background;
        wxColour fgColor = DarkTheme::Text;
        
        if (it->isPressed)
        {
            bgColor = DarkTheme::AccentDark;
            fgColor = wxColour(255, 255, 255);
        }
        else if (it->isHovered)
        {
            bgColor = DarkTheme::Accent;
            fgColor = wxColour(255, 255, 255);
        }
        else if (actualButton == ControlButton::Close)
        {
            bgColor = wxColour(200, 50, 50);
            fgColor = wxColour(255, 255, 255);
        }
        
        dc.SetBrush(wxBrush(bgColor));
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.DrawRoundedRectangle(it->bounds, 3);
        
        // Draw button icon
        dc.SetTextForeground(fgColor);
        dc.SetFont(wxFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        
        wxString symbol;
        switch (actualButton)
        {
            case ControlButton::Minimize:
                symbol = wxT("-");
                break;
            case ControlButton::Maximize:
                symbol = wxT("+");
                break;
            case ControlButton::Restore:
                symbol = wxT("[]");
                break;
            case ControlButton::Close:
                symbol = wxT("X");
                break;
        }
        
        wxSize textSize = dc.GetTextExtent(symbol);
        dc.DrawText(symbol, it->bounds.GetLeft() + (buttonSize - textSize.GetWidth()) / 2,
                    it->bounds.GetTop() + (buttonSize - textSize.GetHeight()) / 2);
    }
}

void CustomMenuBar::OnMouseDown(wxMouseEvent& event)
{
    wxPoint pos = event.GetPosition();
    
    // Check if clicking on window controls
    for (auto& control : windowControls)
    {
        if (control.bounds.Contains(pos))
        {
            control.isPressed = true;
            Refresh();
            return;
        }
    }
    
    // Check if a menu was clicked
    for (size_t i = 0; i < menus.size(); i++)
    {
        if (menus[i].bounds.Contains(pos))
        {
            // Close any existing popup
            if (activePopup)
            {
                activePopup->Hide();
                activePopup = nullptr;
            }
            
            // Toggle menu
            if (menus[i].isOpen)
            {
                menus[i].isOpen = false;
            }
            else
            {
                // Close all other menus
                for (auto& menu : menus)
                    menu.isOpen = false;
                menus[i].isOpen = true;
                
                // Show popup menu at the bottom of the menu item
                wxPoint menuPos = menus[i].bounds.GetBottomLeft();
                menuPos = ClientToScreen(menuPos);
                
                ShowMenuPopup(static_cast<int>(i), menuPos);
            }
            
            Refresh();
            return;
        }
    }
    
    // Check if clicking on title bar area for dragging
    if (pos.y < titleBarHeight && event.LeftIsDown())
    {
        isDragging = true;
        dragStartPos = ClientToScreen(pos);
        frameStartPos = parentFrame->GetPosition();
        CaptureMouse();
    }
}

void CustomMenuBar::OnMouseUp(wxMouseEvent& event)
{
    wxPoint pos = event.GetPosition();
    
    // Handle window control button clicks
    for (auto& control : windowControls)
    {
        if (control.bounds.Contains(pos) && control.isPressed && control.handler)
        {
            wxCommandEvent evt;
            control.handler(evt);
        }
        control.isPressed = false;
    }
    
    // Release mouse capture if we were dragging
    if (isDragging && HasCapture())
    {
        ReleaseMouse();
    }
    
    isDragging = false;
    Refresh();
}

void CustomMenuBar::OnMouseMotion(wxMouseEvent& event)
{
    wxPoint pos = event.GetPosition();
    
    // Update hover state for window controls
    bool anyHovered = false;
    for (auto& control : windowControls)
    {
        if (control.bounds.Contains(pos))
        {
            if (!control.isHovered)
            {
                control.isHovered = true;
                anyHovered = true;
            }
        }
        else
        {
            control.isHovered = false;
        }
    }
    
    // Handle dragging only if left button is held
    if (isDragging && event.LeftIsDown() && parentFrame)
    {
        wxPoint currentPos = ClientToScreen(pos);
        wxPoint delta = currentPos - dragStartPos;
        parentFrame->Move(frameStartPos + delta);
    }
    else
    {
        isDragging = false;
    }
    
    if (anyHovered || isDragging)
        Refresh();
}

void CustomMenuBar::OnLeaveWindow(wxMouseEvent& event)
{
    // Clear hover states
    for (auto& control : windowControls)
    {
        control.isHovered = false;
    }
    
    // Don't close the popup immediately when leaving the menu bar
    // The popup will handle its own closing logic when mouse leaves it
    Refresh();
}
