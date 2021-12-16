#pragma once
#include "Includes.h"

void DrawMenu(int mode)
{
    static bool bMenuOpen = 0;
    if (GetAsyncKeyState(VK_INSERT) & 1) {
        bMenuOpen = !bMenuOpen;
    }
    static int menuOriginX = 300;
    static int menuOriginY = 120;
    const int menuWidth = 800;
    const int menuHeight = 450;
    if ((mode & PAINT_UIPANELS) && bMenuOpen) {
        surface->SetDrawColor(white);
        surface->DrawOutlinedRect(menuOriginX - 1, menuOriginY - 1, menuOriginX + menuWidth + 1, menuOriginY + 1 + menuHeight);
        surface->SetDrawColor(dark_gray);
        surface->DrawFilledRect(menuOriginX, menuOriginY, menuOriginX + menuWidth, menuOriginY + menuHeight);
        surface->SetDrawColor(dark_gray.r, dark_gray.g, dark_gray.b, 255);
        surface->DrawFilledRect(menuOriginX + 20, menuOriginY + 20, menuOriginX + 40, menuOriginY + 40); // drow bhop toggle button
        if (bBhop) {
            surface->SetDrawColor(white.r - 50,white.g-50,white.b-50); 
            surface->DrawFilledRect(menuOriginX + 22, menuOriginY + 22, menuOriginX + 38, menuOriginY + 38); // drow bhop toggle button filling
        }
    }
}