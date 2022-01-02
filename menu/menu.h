@ - 5, 8 + 5, 8 @@
//#include "images/lodepng/lodepng.h"
//#include "images/lodepng/lodepng.cpp"

bool bIndicators = 1;
bool btriggerbot = 0;
static bool bIndicators = 1;
static bool btriggerbot = 0;

/*
std::uint8_t* load_image(unsigned short id, int width, int height, const std::string& type = "PNG")
@ -65,10 +65,18 @@ Code:
loli.paint( 100, 100 );
*/

void drawText(vgui::HFont font, int x, int y, const wchar_t* text, vgui::Color color, int size = 24, const char* fontname = "Tahoma", int weight = 400, int fontflags = 0) {
    void centerText(vgui::HFont font, int& x, int& y, const wchar_t* text) {
        int w;
        int h;
        surface->GetTextSize(font, text, w, h);
        x -= w / 2;
    }

    void drawText(vgui::HFont font, int x, int y, const wchar_t* text, vgui::Color color, int size = 24, const char* fontname = "Tahoma", int weight = 400, int fontflags = 0, bool shouldCenter = 0) {
        surface->SetTextFont(font);
        surface->SetFontGlyphSet(font, fontname, size, weight, 0, 0, fontflags);
        surface->SetTextColor(color.r, color.g, color.b, color.a);
        if (shouldCenter) centerText(font, x, y, text);
        surface->SetTextPosition(x, y);
        surface->PrintText(text, std::wcslen(text));
    }
@ - 88, 7 + 96, 7 @@ public:
    Button(int mStartX, int mStartY, int mEndX, int mEndY, POINT mCursor);

    bool click() {
        return (InRange(cursor.x, start.x, end.x) && InRange(cursor.y, start.y, end.y) && (GetAsyncKeyState(VK_LBUTTON) & 1));
        return (InRange(cursor.x, start.x, end.x) && InRange(cursor.y, start.y, end.y) && (GetAsyncKeyState(VK_LBUTTON) & 1) && bMenuOpen);
    }
};

@ - 193, 7 + 201, 11 @@ void DrawMenu()
GetCursorPos(&cTemp);
cursorX = cTemp.x;
cursorY = cTemp.y;
<<<<<< < HEAD
    drawText(HFMenuTitle, 20, menuOriginY + 17, (const wchar_t*)L"General Toggles", valwhite, 36, "Tahoma", 400);
====== =
surface->SetDrawColor(valred);
>>>>>> > d4ed951a13cee571e63797060e3eca922b4d979e
Button Indicators(menuOriginX + 20, menuOriginY + 65, menuOriginX + 40, menuOriginY + 85, cTemp);
Button triggerbot(menuOriginX + 20, menuOriginY + 90, menuOriginX + 40, menuOriginY + 110, cTemp);
Slider fov(Indicators.start.x + 30, Indicators.start.y + 5, Indicators.start.x + 130, Indicators.start.y + 15, 170, gFov, cTemp);
@ - 204, 7 + 216, 7 @@ void DrawMenu()
surface->DrawFilledRect(menuOriginX, menuOriginY, menuOriginX + menuWidth, menuOriginY + menuHeight);

//Header for general features
drawText(HFMenuTitle, menuOriginX + 20, menuOriginY + 17, (const wchar_t*)L"General Toggles", white, 36, "Tahoma", 400);
drawText(HFMenuTitle, menuOriginX + 20, menuOriginY + 17, L"General Toggles", white, 36, "Tahoma", 400);

//draw indicators button and check for click
surface->SetDrawColor(valgrey);
