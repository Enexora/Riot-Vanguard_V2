#pragma once
#include "Includes.h"

void drawText(vgui::HFont font, int x, int y, const wchar_t* text, vgui::Color color, int size = 24, const char* fontname="Tahoma", int weight=400, int fontflags= surface->FONTFLAG_DROPSHADOW | surface->FONTFLAG_OUTLINE) {
    surface->SetTextFont(font);
    surface->SetFontGlyphSet(font, fontname, size, weight, 0, 0, fontflags | surface->FONTFLAG_DROPSHADOW | surface->FONTFLAG_OUTLINE);
    surface->SetTextColor(color.r, color.g, color.b, color.a);
    surface->SetTextPosition(x, y);
    surface->PrintText(text, std::wcslen(text));
}

void fontInit(vgui::HFont& font, const char* fontname, bool& toggle) {
    font = surface->sCreateFont();
    surface->SetFontGlyphSet(font, fontname, 24, 400, 0, 0, surface->FONTFLAG_DROPSHADOW | surface->FONTFLAG_OUTLINE);
    toggle = 1;
}

class Button {
public:
    POINT start;
    POINT end;
    POINT cursor;
    Button(POINT mStart, POINT mEnd, POINT mCursor);
    Button(int mStartX, int mStartY, int mEndX, int mEndY, POINT mCursor);

    bool click() {
        return (InRange(cursor.x, start.x, end.x) && InRange(cursor.y, start.y, end.y) && (GetAsyncKeyState(VK_LBUTTON) & 1));
    }
};
Button::Button (POINT mStart, POINT mEnd, POINT mCursor) {
    start = mStart;
    end = mEnd;
    cursor = mCursor;
}
Button::Button (int mStartX, int mStartY, int mEndX, int mEndY, POINT mCursor) {
    start.x = mStartX;
    start.y = mStartY;
    end.x = mEndX;
    end.y = mEndY;
    cursor = mCursor;
}

class Slider {
public:
    POINT start;
    POINT end;
    int multiplier;
    int value;
    POINT cursor;
    Slider(POINT mStart, POINT mEnd, int multiplier, int value, POINT mCursor);
    Slider(int mStartX, int mStartY, int mEndX, int mEndY, int mMultiplier, int value, POINT mCursor);
    float click() {
        if (InRange(cursor.x, start.x, start.x + 100) && InRange(cursor.y, start.y, start.y + 10) && GetAsyncKeyState(VK_LBUTTON)) {
            return 0.1f + (cursor.x - start.x) * multiplier / 100;
        }
        return 0;
    }
};

Slider::Slider(POINT mStart, POINT mEnd, int mMultiplier, int value, POINT mCursor) {
    start = mStart;
    end = mEnd;
    multiplier = mMultiplier;
    cursor = mCursor;
    end.x = start.x + (100 * value / 179);
}

Slider::Slider(int mStartX, int mStartY, int mEndX, int mEndY, int mMultiplier, int value, POINT mCursor) {
    start.x = mStartX;
    start.y = mStartY;
    end.x = start.x + (100 * value / 179);
    end.y = mEndY;
    multiplier = mMultiplier;
    cursor = mCursor;
}

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
    Button bhop(menuOriginX + 20, menuOriginY + 65, menuOriginX + 40, menuOriginY + 85, cTemp);
    Slider fov(bhop.start.x + 30, bhop.start.y + 5, bhop.start.x + 130, bhop.start.y + 15, 179, gFov, cTemp);
    if ((mode & PAINT_UIPANELS) && bMenuOpen) {
        //draw main menu bg and outline
        surface->SetDrawColor(white);
        surface->DrawOutlinedRect(menuOriginX - 1, menuOriginY - 1, menuOriginX + menuWidth + 1, menuOriginY + 1 + menuHeight);
        surface->SetDrawColor(dark_gray);
        surface->DrawFilledRect(menuOriginX, menuOriginY, menuOriginX + menuWidth, menuOriginY + menuHeight);

        //Header for general features
        drawText(Tahoma, menuOriginX + 20, menuOriginY + 17, (const wchar_t*)L"General Toggles", white, 36, "Arial", 700, surface->FONTFLAG_GAUSSIANBLUR);

        //draw bhop button and check for click
        surface->SetDrawColor(black);
        surface->DrawFilledRect(bhop.start.x, bhop.start.y, bhop.end.x, bhop.end.y); // drow bhop toggle button
        if (bhop.click()) bBhop = !bBhop;
        if (bBhop) {
            surface->SetDrawColor(white);
            surface->DrawFilledRect(bhop.start.x+2, bhop.start.y + 2, bhop.end.x-2, bhop.end.y-2); // drow bhop toggle button 
        }

        //draw fov slider
        surface->SetDrawColor(red);
        surface->DrawFilledRect(fov.start.x, fov.start.y, fov.end.x, fov.end.y); // actual slider for fov
        surface->SetDrawColor(white);
        surface->DrawOutlinedRect(fov.start.x, fov.start.y, fov.start.x + 100, fov.end.y);
        if (fov.click() > .1f) {
            gFov = fov.click();
        }
        drawText(Tahoma, fov.start.x + 105, fov.start.y - 3, std::to_wstring((int)gFov).c_str(), white, 16); // text position is based from top left corner, slightly below it


    }
}