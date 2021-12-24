// dllmain.cpp : Defines the entry point for the DLL application.

#include "includes.h"

DWORD WINAPI bhop(HMODULE hModule) {
    DWORD ulOldProtect[5];
    client = (DWORD)GetModuleHandle("client.dll");
    engine = (DWORD)GetModuleHandle("engine.dll");
    vguimatsurface = (DWORD)GetModuleHandle("vguimatsurface.dll");
    finishDrawing = (tFinishDrawing)(pattern_scan("vguimatsurface.dll", "8B 0D ? ? ? ? 56 C6 05"));
    startDrawing = (tStartDrawing)pattern_scan("vguimatsurface.dll", "55 8B EC 83 E4 C0 83 EC 38");
    clientMode = **(ClientModeShared***)((*(uintptr_t**)(pClientDLL))[10] + 0x5);
    input = *(CInput**)((*(DWORD**)pClientDLL)[16] + 1);
    Globals = **(CGlobals***)((*(DWORD**)pClientDLL)[11] + 0xA);
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
    fCreateMove = (tCreateMove)*(DWORD*)(vtable + 24*sizeof(void*));
    fLoadSkybox = (tLoadSkybox)(engine + 0x131720);
    fFrameStageNotify = *(tFrameStageNotify*)(*(DWORD*)(pClientDLL) + 37*sizeof(void*));
    fPaint = (tPaint)*(DWORD*)((*(DWORD*)engineVGui) + (14 * sizeof(void*)));
    fOverrideView = (tOverrideView)*(DWORD*)(vtable + 18*sizeof(void*));
    fLockCursor = (tLockCursor)*(DWORD*)((*(DWORD*)surface) + 67 * sizeof(void*));
    *(DWORD*)((*(DWORD*)surface) + 67 * sizeof(void*)) = (DWORD)&hkLockCursor;
    *(DWORD*)((*(DWORD*)engineVGui) + (14 * sizeof(void*))) = (DWORD)&hkPaint;
    *(DWORD*)(*(DWORD*)(pClientDLL)+ 37*sizeof(void*)) = (DWORD)&hkFrameStageNotify;
    *(DWORD*)(vtable + 24*sizeof(void*)) = (DWORD)&hkCreateMove;
    *(DWORD*)(vtable + 18*sizeof(void*)) = (DWORD)&hkOverrideView;
    EngineClient->ClientCmdUnrestricted("playvol ambient/flamenco.wav .25");
    while (!GetAsyncKeyState(VK_END)){
        localPlayer = *(Player**)(client + dwLocalPlayer);
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
        if (!EngineClient->IsInGame()) continue;
        if (localPlayer == NULL) continue;
        if (EngineClient->GetNetChannelInfo() != netchan) {
            netchan = EngineClient->GetNetChannelInfo();
        }
        for (int i = 0; i < 48; i++) {
            Player* ent = (Player*)EntityList->GetClientEntity(i);
                if (ent == NULL) continue;
                if (ent->m_iTeamNum() != localPlayer->m_iTeamNum()) *(bool*)(ent + m_bSpotted) = true;
                if (*(float*)((DWORD)localPlayer + m_flFlashDuration) > 0)
                {
                    *(float*)((DWORD)localPlayer + m_flFlashDuration) = 0;
                }
                if(bEsp && ent->m_iHealth()){
                    int glowIndex = *(int*)((DWORD)ent + m_iGlowIndex);
                    if (ent->m_iTeamNum() != localPlayer->m_iTeamNum()) {
                        *(float*)(glowObj + glowIndex * 0x38 + 0x8) = 1.f;
                        *(float*)(glowObj + glowIndex * 0x38 + 0xC) = 0.15f;
                        *(float*)(glowObj + glowIndex * 0x38 + 0x10) = (ent->m_iHealth() / 200.f);
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
        Sleep(1);
    }
    toggleAll();
    *SendPacket = true;
    *(DWORD*)((*(DWORD*)engineVGui) + (14 * sizeof(void*))) = (DWORD)fPaint;
    *(DWORD*)(*(DWORD*)(pClientDLL)+37 * sizeof(void*)) = (DWORD)fFrameStageNotify;
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

