#pragma once
#include "Includes.h"

void DrawMenu(int mode)
{
    if (GetAsyncKeyState(VK_INSERT) & 1) {
        bMenuOpen = !bMenuOpen;
    }
    static int cursorX = 0;
    static int cursorY = 0;
    surface->SurfaceGetCursorPos(cursorX, cursorY);
    static int menuOriginX = 300;
    static int menuOriginY = 120;
    constexpr int menuWidth = 800;
    constexpr int menuHeight = 450;
    POINT cTemp;
    GetCursorPos(&cTemp);
    cursorX = cTemp.x;
    cursorY = cTemp.y;
    if ((mode & PAINT_UIPANELS) && bMenuOpen) {
        surface->SetDrawColor(white);
        surface->DrawOutlinedRect(menuOriginX - 1, menuOriginY - 1, menuOriginX + menuWidth + 1, menuOriginY + 1 + menuHeight);
        surface->SetDrawColor(dark_gray);
        surface->DrawFilledRect(menuOriginX, menuOriginY, menuOriginX + menuWidth, menuOriginY + menuHeight);
        surface->SetDrawColor(black);
        surface->DrawFilledRect(menuOriginX + 20, menuOriginY + 20, menuOriginX + 40, menuOriginY + 40); // drow bhop toggle button
        if (InRange(cursorX, menuOriginX+20, menuOriginX+40) && InRange(cursorY, menuOriginY+20, menuOriginY+40) && (GetAsyncKeyState(VK_LBUTTON) & 1)) {
            bBhop = !bBhop;
        }
        if (bBhop) {
            surface->SetDrawColor(white);
            surface->DrawFilledRect(menuOriginX + 22, menuOriginY + 22, menuOriginX + 38, menuOriginY + 38); // drow bhop toggle button 
        }
    }
}