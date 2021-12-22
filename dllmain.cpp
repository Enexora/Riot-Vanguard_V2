// dllmain.cpp : Defines the entry point for the DLL application.

#include "includes.h"

DWORD WINAPI bhop(HMODULE hModule) {
    DWORD ulOldProtect[5];
    client = (DWORD)GetModuleHandle("client.dll");
    engine = (DWORD)GetModuleHandle("engine.dll");
    vguimatsurface = (DWORD)GetModuleHandle("vguimatsurface.dll");
    finishDrawing = (tFinishDrawing)(vguimatsurface + 0xD560);
    startDrawing = (tStartDrawing)(vguimatsurface + 0xD380);
    clientMode = **(ClientModeShared***)((*(uintptr_t**)(pClientDLL))[10] + 0x5);
    input = *(CInput**)((*(DWORD**)pClientDLL)[16] + 1);
    Globals = *(CGlobals**)((*(DWORD**)pClientDLL)[0] + 0x1F);
    netchan = EngineClient->GetNetChannelInfo();
    // Dumping netvars
    for (auto pClass = pClientDLL->GetAllClasses(); pClass; pClass = pClass->m_pNext) {
        if (pClass->m_pRecvTable) NetVarManager::Get().DumpRecursive(pClass->m_pRecvTable);
    }
    fCL_SendMove = (tSendMove)(engine + 0xD9160);
    VirtualProtect((void*)input, 0xF00, PAGE_EXECUTE_READWRITE, &ulOldProtect[0]);
    VirtualProtect(*(void**)engineVGui, 0xF00, PAGE_EXECUTE_READWRITE, &ulOldProtect[3]);
    VirtualProtect(*(void**)pClientDLL, 0xF00, PAGE_EXECUTE_READWRITE, &ulOldProtect[5]);
    DWORD vtable = *(DWORD*)clientMode;
    SendPacket = (byte*)(engine + dwbSendPackets);
    VirtualProtect(SendPacket, sizeof(SendPacket), PAGE_EXECUTE_READWRITE, 0);
    VirtualProtect((void*)vtable, 0x400, PAGE_EXECUTE_READWRITE, &ulOldProtect[1]);
    VirtualProtect(*(void**)surface, 0xA00, PAGE_EXECUTE_READWRITE, &ulOldProtect[4]);
    //Detour((DWORD*)(engine + 0xD947F), hkSendMove);
    fCreateMove = (tCreateMove)*(DWORD*)(vtable + 24*sizeof(void*));
    fLoadSkybox = (tLoadSkybox)(engine + 0x131720);
    fPaint = (tPaint)*(DWORD*)((*(DWORD*)engineVGui) + (14 * sizeof(void*)));
    //fWriteUsercmdDelta = (tWriteUsercmdDelta)*(DWORD*)(**(DWORD**)(pClientDLL) + (22 * sizeof(void*))); // index 22
    fOverrideView = (tOverrideView)*(DWORD*)(vtable + 18*sizeof(void*));
    fLockCursor = (tLockCursor)*(DWORD*)((*(DWORD*)surface) + 67 * sizeof(void*));
    *(DWORD*)((*(DWORD*)surface) + 67 * sizeof(void*)) = (DWORD)&hkLockCursor;
    *(DWORD*)((*(DWORD*)engineVGui) + (14 * sizeof(void*))) = (DWORD)&hkPaint;
    *(DWORD*)(vtable + 24*sizeof(void*)) = (DWORD)&hkCreateMove;
    *(DWORD*)(vtable + 18*sizeof(void*)) = (DWORD)&hkOverrideView;
    EngineClient->ClientCmdUnrestricted("playvol ambient/flamenco.wav .25");
    while (!GetAsyncKeyState(VK_END)){
        toggle(bAA, VK_NUMPAD5);
        toggle(bBhop, VK_NUMPAD1);
        toggle(bAimbot, VK_NUMPAD4);
        toggle(bEsp, VK_NUMPAD2);
        toggle(bTP, VK_MBUTTON);
        toggle(bBT, VK_NUMPAD3);
        if (GetAsyncKeyState(VK_NUMPAD0) & 1) {
            toggleAll();
        }
        DWORD glowObj = *(DWORD*)(client + dwGlowObjectManager);
        localPlayer = *(DWORD*)(client + dwLocalPlayer);
        Player* plocalPlayer = (Player*)(client + dwLocalPlayer);
        if (!EngineClient->IsInGame()) continue;
        if (localPlayer == NULL) continue;
        if (EngineClient->GetNetChannelInfo() != netchan) {
            netchan = EngineClient->GetNetChannelInfo();
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

