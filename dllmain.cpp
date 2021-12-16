// dllmain.cpp : Defines the entry point for the DLL application.
#include "Includes.h"

DWORD WINAPI bhop(HMODULE hModule) {
    DWORD ulOldProtect[4];
    client = (DWORD)GetModuleHandle("client.dll");
    engine = (DWORD)GetModuleHandle("engine.dll");
    clientMode = **(ClientModeShared***)((*(uintptr_t**)(pClientDLL))[10] + 0x5);
    input = *(CInput**)((*(DWORD**)pClientDLL)[16] + 1);
    globals = *(CGlobals**)((*(DWORD**)pClientDLL)[0] + 0x1F);
    netchan = EngineClient->GetNetChannelInfo();
    VirtualProtect((void*)input, 0xA00, PAGE_EXECUTE_READWRITE, &ulOldProtect[0]);
    VirtualProtect(*(void**)engineVGui, 0xA00, PAGE_EXECUTE_READWRITE, &ulOldProtect[3]);
    DWORD vtable = *(DWORD*)clientMode;
    SendPacket = (byte*)(engine + dwbSendPackets);
    VirtualProtect(SendPacket, sizeof(SendPacket), PAGE_EXECUTE_READWRITE, 0);
    VirtualProtect((void*)vtable, 0x400, PAGE_EXECUTE_READWRITE, &ulOldProtect[1]);
    VirtualProtect(*(void**)surface, 0xA00, PAGE_EXECUTE_READWRITE, &ulOldProtect[4]);
    fCreateMove = (tCreateMove)*(DWORD*)(vtable + 24*sizeof(void*));
    fPaint = (tPaint)*(DWORD*)((*(DWORD*)engineVGui) + (14 * sizeof(void*)));
    fOverrideView = (tOverrideView)*(DWORD*)(vtable + 18*sizeof(void*));
    fLockCursor = (tLockCursor)*(DWORD*)((*(DWORD*)surface) + 67 * sizeof(void*));
    *(DWORD*)((*(DWORD*)surface) + 67 * sizeof(void*)) = (DWORD)&hkLockCursor;
    *(DWORD*)((*(DWORD*)engineVGui) + (14 * sizeof(void*))) = (DWORD)&hkPaint;
    *(DWORD*)(vtable + 24*sizeof(void*)) = (DWORD)&hkCreateMove;
    *(DWORD*)(vtable + 18*sizeof(void*)) = (DWORD)&hkOverrideView;
    EngineClient->ExecuteClientCmd("playvol ambient/flamenco.wav .25");
    while (!GetAsyncKeyState(VK_END)){
        DWORD glowObj = *(DWORD*)(client + dwGlowObjectManager);
        localPlayer = *(DWORD*)(client + dwLocalPlayer);
        DWORD* plocalPlayer = (DWORD*)(client + dwLocalPlayer);
        if (!EngineClient->IsInGame()) continue;
        if (localPlayer == NULL) continue;
        if (EngineClient->GetNetChannelInfo() != netchan) {
            netchan = EngineClient->GetNetChannelInfo();
        }
        toggle(bAA, VK_NUMPAD5);
        toggle(bBhop, VK_NUMPAD1);
        toggle(bAimbot, VK_NUMPAD4);
        toggle(bEsp, VK_NUMPAD2);
        toggle(bTP, VK_MBUTTON);
        toggle(bBT, VK_NUMPAD3);
        if (GetAsyncKeyState(VK_NUMPAD0) & 1) {
            toggleAll();
        }
        DWORD flags = *(int*)(localPlayer + m_fFlags);
        for (int i = 0; i < 64; i++) {
            DWORD ent = *(DWORD*)(client + dwEntityList + i * 0x10);
                if (ent == NULL) continue;
                if (*(int*)(ent + m_iTeamNum) != *(int*)(localPlayer + m_iTeamNum)) *(bool*)(ent + m_bSpotted) = true;
                if(bEsp){
                int glowIndex = *(int*)(ent + m_iGlowIndex);
                if (*(int*)(ent + m_iTeamNum) != *(int*)(localPlayer + m_iTeamNum)) {
                    *(float*)(glowObj + glowIndex * 0x38 + 0x8) = 1.f;
                    *(float*)(glowObj + glowIndex * 0x38 + 0xC) = 0.15f;
                    *(float*)(glowObj + glowIndex * 0x38 + 0x10) = (*(int*)(ent + m_iHealth) / 200.f);
                    *(float*)(glowObj + glowIndex * 0x38 + 0x14) = 1.f;
                }
                else
                {
                    *(float*)(glowObj + glowIndex * 0x38 + 0x8) = 0.f;
                    *(float*)(glowObj + glowIndex * 0x38 + 0xC) = 0.166f;
                    *(float*)(glowObj + glowIndex * 0x38 + 0x10) = 1.f;
                    *(float*)(glowObj + glowIndex * 0x38 + 0x14) = .6f;
                }
                *(int*)(glowObj + glowIndex * 0x38 + 0x28) = 1;
                *(int*)(glowObj + glowIndex * 0x38 + 0x29) = 0;
            }
        }
        if (*(float*)(localPlayer + m_flFlashDuration) > 0)
        {
            *(float*)(localPlayer + m_flFlashDuration) = 0;
        }
        Sleep(1);
    }
    toggleAll();
    *SendPacket = true;
    *(DWORD*)((*(DWORD*)engineVGui) + (14 * sizeof(void*))) = (DWORD)fPaint;
    *(DWORD*)((*(DWORD*)surface) + 67 * sizeof(void*)) = (DWORD)fLockCursor;
    *(DWORD*)(vtable + 24 * sizeof(void*)) = (DWORD)fCreateMove;
    *(DWORD*)(vtable + 18 * sizeof(void*)) = (DWORD)fOverrideView;
    FreeLibraryAndExitThread(hModule, 0);
};

BOOL WINAPI DllMain( HINSTANCE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)bhop, hModule, 0, 0);
        break;
    }
    return TRUE;
}

