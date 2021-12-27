#pragma once
#include "Includes.h"

void ClanTag()
{
    static const std::string myClanTagArray[2] = { u8"֎Vanguard֎", u8"₪Vanguard₪"};
    static const std::string MONOLOL[29] = { 
        u8"[--------]", 
        u8"[M-------]",
        u8"[Mo------]",
        u8"[Mo------]",
        u8"[Mon-----]",
        u8"[Mono----]",
        u8"[Monoli--]",
        u8"[Monolit-]",
        u8"[Monolith]",
        u8"[Monolith]",
        u8"[Monolith]",
        u8"[Monolith]",
        u8"[Monolith]",
        u8"[>onolith]",
        u8"[1>nolith]",
        u8"[11>olith]",
        u8"[911>lith]",
        u8"[<911>ith]",
        u8"[-<911>th]",
        u8"[--<911>h]",
        u8"[---<911>]",
        u8"[----<911]",
        u8"[-----<91]",
        u8"[------<9]",
        u8"[-------<]",
        u8"[--------]",
        u8"[--------]",
        u8"[--------]",
        u8"[--------]", 
    };
    using tSetClanTag = void(__fastcall*)(const char*, const char*);
    static tSetClanTag SetClanTag = (tSetClanTag)(engine + dwSetClanTag);
    DWORD* pClientDLL = (DWORD*)GetInterface("client.dll", "VClient018");
    static int lastTick = 0;
    if (abs(Globals->tick_count - lastTick) >= 25)
    {
        static int i;
        lastTick = Globals->tick_count;
        SetClanTag(myClanTagArray[i % 2].c_str(), "garbage empty bullshit");
        //SetClanTag(MONOLOL[i % 30].c_str(), "garbage empty bullshit");
        i++;
    }
}