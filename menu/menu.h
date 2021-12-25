#pragma once

#include "includes.h"
#include "menu/images/resource.h"
//#include "images/lodepng/lodepng.h"
//#include "images/lodepng/lodepng.cpp"

bool bIndicators = 1;
bool btriggerbot = 0;

/*
std::uint8_t* load_image(unsigned short id, int width, int height, const std::string& type = "PNG")
{
    // csgo->module is the module you get from creating a thread in DLLMain
    // static DWORD __stdcall thread( const LPVOID parameter )
    // csgo->module = HMODULE( parameter )
    auto resource = FindResource(csgo->module, MAKEINTRESOURCE(id), type.data());
    auto loaded_resource = LoadResource(csgo->module, resource);
    auto resource_ptr = LockResource(loaded_resource);
    auto resource_size = SizeofResource(csgo->module, resource);
    std::vector< std::uint8_t > image;
    auto w = std::uint32_t(width);
    auto h = std::uint32_t(height);
    if (const auto error = lodepng::decode(image, w, h, (unsigned char*)resource_ptr, resource_size))
        exit(0);
    const auto data = new std::uint8_t[image.size()];
    std::copy(image.begin(), image.end(), data);
    return data;
}
class c_texture
{
    // raw image data
    unsigned char* data_{ };
    // ISurface texture id
    unsigned int texture_id_{ 0 };
    // size
    int width_{ };
    int height_{ };
public:
    c_texture() = default;
    // create texture
    c_texture(unsigned short id, int width, int height, const std::string& type = "PNG")
        : data_{ load_image(id, width, height, type) }
        , width_(width)
        , height_(height)
    { }
    // draw texture
    void paint(int x, int y)
    {
        if (!surface->is_texture_id_valid(texture_id_))
        {
            texture_id_ = surface->create_new_texture_id(true);
            surface->draw_set_texture_rgba(texture_id_, data_, uint32_t(width_), uint32_t(height_));
        }
        surface->draw_set_color(255, 255, 255, 255);
        surface->draw_set_texture(texture_id_);
        surface->draw_textured_rect(x, y, x + width_, y + height_);
    }
};
Then all you have to do is create a texture:
Code:
loli = c_texture( LOLI, 540, 474 );
And then draw it:
Code:
loli.paint( 100, 100 );
*/

void drawText(vgui::HFont font, int x, int y, const wchar_t* text, vgui::Color color, int size = 24, const char* fontname = "Tahoma", int weight = 400, int fontflags = 0) {
    surface->SetTextFont(font);
    surface->SetFontGlyphSet(font, fontname, size, weight, 0, 0, fontflags);
    surface->SetTextColor(color.r, color.g, color.b, color.a);
    surface->SetTextPosition(x, y);
    surface->PrintText(text, std::wcslen(text));
}

void fontInit(vgui::HFont& font, const char* fontname, bool& toggle) {
    font = surface->sCreateFont();
    surface->SetFontGlyphSet(font, fontname, 24, 400, 0, 0, FONTFLAG_OUTLINE);
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

Button::Button(POINT mStart, POINT mEnd, POINT mCursor) {
    start = mStart;
    end = mEnd;
    cursor = mCursor;
}
Button::Button(int mStartX, int mStartY, int mEndX, int mEndY, POINT mCursor) {
    start.x = mStartX;
    start.y = mStartY;
    end.x = mEndX;
    end.y = mEndY;
    cursor = mCursor;
}

class cFont {
public:
    cFont() {};
    vgui::HFont hfont;


};

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
    end.x = start.x + (100 * value / mMultiplier);
}

Slider::Slider(int mStartX, int mStartY, int mEndX, int mEndY, int mMultiplier, int value, POINT mCursor) {
    start.x = mStartX;
    start.y = mStartY;
    end.x = start.x + (100 * value / mMultiplier);
    end.y = mEndY;
    multiplier = mMultiplier;
    cursor = mCursor;
}

vgui::Color valred = { 255, 70, 84, 255 };
vgui::Color valblack = { 27, 32, 42, 255 };
vgui::Color valgrey = { 44, 48, 53, 255 };
vgui::Color valwhite = { 236, 233, 225, 255 };


struct Indicators_t {
    std::wstring name;
    bool active;
};

void DrawIndicators() {
    if (!bIndicators) return;

    int i = 350;

    std::vector<Indicators_t> temp;
    temp.push_back(Indicators_t{ L"BHOP", bBhop });
    temp.push_back(Indicators_t{ L"ESP", bEsp });
    temp.push_back(Indicators_t{ L"BACKTRACK", bBT });
    temp.push_back(Indicators_t{ L"AIMBOT", bAimbot });
    temp.push_back(Indicators_t{ L"ANTI-AIM", bAA });

    for (auto& item : temp) {
        drawText(HFIndicators, 5, i, item.name.data(), item.active ? green : red, 24, "Tahoma", 600, FONTFLAG_DROPSHADOW);
        i += 20;
    }
}

void DrawMenu()
{
    if (GetAsyncKeyState(VK_INSERT) & 1) {
        bMenuOpen = !bMenuOpen;
    }
    if (!bMenuOpen) return;
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
    surface->SetDrawColor(valred);
    Button Indicators(menuOriginX + 20, menuOriginY + 65, menuOriginX + 40, menuOriginY + 85, cTemp);
    Button triggerbot(menuOriginX + 20, menuOriginY + 90, menuOriginX + 40, menuOriginY + 110, cTemp);
    Slider fov(Indicators.start.x + 30, Indicators.start.y + 5, Indicators.start.x + 130, Indicators.start.y + 15, 170, gFov, cTemp);
    //draw main menu bg and outline
    surface->SetDrawColor(valred);
    surface->DrawOutlinedRect(menuOriginX - 1, menuOriginY - 1, menuOriginX + menuWidth + 1, menuOriginY + 1 + menuHeight);
    surface->SetDrawColor(valblack);
    surface->DrawFilledRect(menuOriginX, menuOriginY, menuOriginX + menuWidth, menuOriginY + menuHeight);

    //Header for general features
    drawText(HFMenuTitle, menuOriginX + 20, menuOriginY + 17, (const wchar_t*)L"General Toggles", white, 36, "Tahoma", 400);

    //draw indicators button and check for click
    surface->SetDrawColor(valgrey);
    surface->DrawFilledRect(Indicators.start.x, Indicators.start.y, Indicators.end.x, Indicators.end.y); // drow bhop toggle button
    surface->SetDrawColor(valwhite);
    surface->DrawOutlinedRect(Indicators.start.x, Indicators.start.y, Indicators.end.x, Indicators.end.y);
    if (Indicators.click()) bIndicators = !bIndicators;
    if (bIndicators) {
        surface->SetDrawColor(white);
        surface->DrawFilledRect(Indicators.start.x + 3, Indicators.start.y + 3, Indicators.end.x - 3, Indicators.end.y - 3); // drow bhop toggle button 
    }
    //draw botriger button and check for click
    surface->SetDrawColor(valgrey);
    surface->DrawFilledRect(triggerbot.start.x, triggerbot.start.y, triggerbot.end.x, triggerbot.end.y); // toggle triggerobt
    surface->SetDrawColor(valwhite);
    surface->DrawOutlinedRect(triggerbot.start.x, triggerbot.start.y, triggerbot.end.x, triggerbot.end.y);
    if (triggerbot.click()) btriggerbot = !btriggerbot;
    if (btriggerbot) {
        surface->SetDrawColor(white);
        surface->DrawFilledRect(triggerbot.start.x + 3, triggerbot.start.y + 3, triggerbot.end.x - 3, triggerbot.end.y - 3); // toggle triggerobt 
    }


    //draw fov slider
    surface->SetDrawColor(valgrey);
    surface->DrawFilledRect(fov.start.x, fov.start.y, fov.start.x + 100, fov.end.y);
    surface->SetDrawColor(valred);
    surface->DrawFilledRect(fov.start.x, fov.start.y, fov.end.x, fov.end.y); // actual slider for fov
    surface->SetDrawColor(valgrey);
    surface->DrawOutlinedRect(fov.start.x, fov.start.y, fov.start.x + 100, fov.end.y);
    if (fov.click() > .1f) {
        gFov = fov.click();
    }
    if (gFov <= 0) gFov = .1f;
    drawText(HFMenuSliders, fov.start.x + 105, fov.start.y - 4, std::to_wstring((int)gFov).c_str(), white, 16); // text position is based from top left corner, slightly below it
}

/*
void on_paint()
{
    auto curtime = GlobalVars->curtime;
    auto lineSize = 8;
    if (hurtTime + 0.25f >= curtime) {
        int screenSizeX, screenCenterX;
        int screenSizeY, screenCenterY;
        EngineClient->GetScreenSize(screenSizeX, screenSizeY);

        screenCenterX = screenSizeX / 2;
        screenCenterY = screenSizeY / 2;

        surface->SetDrawColor(valred);
        surface->DrawLine(screenCenterX - lineSize, screenCenterY - lineSize, screenCenterX - (lineSize / 4), screenCenterY - (lineSize / 4));
        surface->DrawLine(screenCenterX - lineSize, screenCenterY + lineSize, screenCenterX - (lineSize / 4), screenCenterY + (lineSize / 4));
        surface->DrawLine(screenCenterX + lineSize, screenCenterY + lineSize, screenCenterX + (lineSize / 4), screenCenterY + (lineSize / 4));
        surface->DrawLine(screenCenterX + lineSize, screenCenterY - lineSize, screenCenterX + (lineSize / 4), screenCenterY - (lineSize / 4));
    }
}
*/

