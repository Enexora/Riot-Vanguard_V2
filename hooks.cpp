#pragma once
#include "menu.h"


DWORD* pVTableBase = 0;
DWORD* pVTableFnc = 0;
QAngle ViewAngles;
QAngle aimbotAngles = { 0,0,0 };
QAngle prevAngles = { 0,0,0 };
float fovAimbot = 15.f;
bool flip = false;
float lastTime;

struct btRecord
{
    int tick;
    float magnitude;
    Vector3 position;
};

void Detour(void* src, void* dst) {
    static DWORD oldProtect;
    VirtualProtect(src, 0x5, PAGE_EXECUTE_READWRITE, &oldProtect);
    *(BYTE*)(src) = 0xE8;
    *(DWORD*)((DWORD)src + 1) = ((DWORD)dst - (DWORD)src - 5);
    VirtualProtect(src, 0x5, oldProtect, 0);
}

void FixMovement(CUserCmd* cmd, IEngineClient* Engine, QAngle viewangles)
{
    Vector3 vecMove = { cmd->forwardmove, cmd->sidemove, cmd->upmove };
    float speed = sqrt(vecMove.x * vecMove.x + vecMove.y * vecMove.y);

    Vector3 angMove;
    VectorAngles(vecMove, angMove);

    float yaw = DEG2RAD(cmd->viewangles.yaw - viewangles.yaw + angMove.y);
    if (speed != 0) {
        cmd->forwardmove = cos(yaw) * speed;
        cmd->sidemove = sin(yaw) * speed;
    }
}

void SlowWalk(CUserCmd* cmd, float forwardSpeed, float sideSpeed) {
    if (sideSpeed > 39) { cmd->sidemove = 39; }
    if (sideSpeed < -39) { cmd->sidemove = -39; }
    if (forwardSpeed > 39) { cmd->forwardmove = 39; }
    if (forwardSpeed < -39) { cmd->forwardmove = -39; }
}
static QAngle cmdView = { 0,0,0 };

bool __fastcall hkCreateMove(void* ecx, void* edx, float flSampleTimer, CUserCmd* cmd) {
    ClientState = *(DWORD*)(engine + dwClientState);
    if (!EngineClient->IsInGame()) return 0;
    static bool shotLast = 1;
    static int trollEnt = 0;
    localPlayer = *(DWORD*)(client + dwLocalPlayer);
    static btRecord backtrack;
    static int btTick = 0;
    static QAngle btAngles = { 0,0,0 };
    static Vector3 btEntPos{ 0,0,0 };
    static float btMagnitude = 0.f;
    if (localPlayer == NULL) return 0;
    DWORD flags = *(int*)(localPlayer + m_fFlags);
    ViewAngles = *(QAngle*)(ClientState + dwClientState_ViewAngles);
    QAngle punchAngle = *(QAngle*)(localPlayer + m_aimPunchAngle);
    if (bBhop == true) {
        if (cmd->buttons & IN_JUMP) {
            if (flags & FL_ONGROUND) {
                cmd->buttons |= IN_JUMP;
            }
            else
            {
                cmd->buttons &= ~IN_JUMP;
            }
        }
    }
    DWORD initWep = *(DWORD*)(localPlayer + m_hActiveWeapon) & 0xFFF;
    DWORD wepEntity = *(DWORD*)(client + dwEntityList + (initWep - 1) * 0x10);
    float sideSpeed = cmd->sidemove;
    float forwardSpeed = cmd->forwardmove;
    Vector3 PlayerPos = *(Vector3*)(localPlayer + m_vecOrigin);
    static float entsim;
        for (int i = 0; i < 64; i++) {                                                                              // Ent list for aimbot
            DWORD ent = *(DWORD*)(client + dwEntityList + i * 0x10);
            if (ent == NULL) {
                aimbotAngles = ViewAngles;
                continue;
            }
            Vector3 entHPos = *(Vector3*)(ent + m_vecOrigin);
            Vector3 entVelocity = *(Vector3*)(ent + m_vecVelocity);
            PlayerPos = *(Vector3*)(localPlayer + m_vecOrigin);
            float trollmagnitude = sqrt(((entHPos.x - PlayerPos.x) * (entHPos.x - PlayerPos.x)) + ((entHPos.y - PlayerPos.y) * (entHPos.y - PlayerPos.y)));
            if(*(int*)(localPlayer + m_iCrosshairId) != 0){
                if (GetAsyncKeyState(0x54) & 1) {
                    trollEnt = *(int*)(localPlayer + m_iCrosshairId) - 1;
                }
            }
            if (trollEnt == i && *(DWORD*)(client + dwEntityList + trollEnt * 0x10) != localPlayer && *(int*)(*(DWORD*)(client + dwEntityList + trollEnt * 0x10) + m_iHealth) > 0 && GetAsyncKeyState(0x46)){
                cmd->forwardmove = clamp450(cos(DEG2RAD(ViewAngles.yaw)) * (250 * (entHPos.x - PlayerPos.x) + entVelocity.x) + sin(DEG2RAD(ViewAngles.yaw)) * (250 * (entHPos.y - PlayerPos.y) + entVelocity.y));
                cmd->sidemove = clamp450(cos(DEG2RAD(ViewAngles.yaw)) * -(250 * (entHPos.y - PlayerPos.y) + entVelocity.y) + sin(DEG2RAD(ViewAngles.yaw)) * (250 * (entHPos.x - PlayerPos.x) + entVelocity.x));
            }
            entHPos.x = *(float*)(*(DWORD*)(ent + m_dwBoneMatrix) + 0x30 * 8 + 0x0C);
            entHPos.y = *(float*)(*(DWORD*)(ent + m_dwBoneMatrix) + 0x30 * 8 + 0x1C);
            entHPos.z = *(float*)(*(DWORD*)(ent + m_dwBoneMatrix) + 0x30 * 8 + 0x2C);
            PlayerPos.z += *(float*)(localPlayer + m_vecViewOffset + 0x8);
            if (bAimbot == true) {
            if (*(bool*)(ent + m_bDormant) == 1 || ent == localPlayer || *(int*)(ent + m_iTeamNum) == *(int*)(localPlayer + m_iTeamNum)) {
                aimbotAngles = ViewAngles;
                continue;
            }
            if (*(int*)(ent + m_iHealth) <= 0) {
                aimbotAngles = ViewAngles;
                continue;
            }
            float magnitude = sqrt(((entHPos.x - PlayerPos.x) * (entHPos.x - PlayerPos.x)) + ((entHPos.y - PlayerPos.y) * (entHPos.y - PlayerPos.y)));
            if (magnitude == 0) {
                continue;
            }
            aimbotAngles.pitch = (180.f * (-atan((entHPos.z - PlayerPos.z) / (magnitude))) / PI) - (2.f * punchAngle.pitch);
            aimbotAngles.yaw = 180.f * (atan2(entHPos.y - PlayerPos.y, entHPos.x - PlayerPos.x)) / PI - (2.f * punchAngle.yaw);
            clamp89(aimbotAngles.pitch);
            clamp180(aimbotAngles.yaw);
            if (abs(ViewAngles.yaw - prevAngles.yaw) + abs(ViewAngles.pitch - prevAngles.pitch) > abs(ViewAngles.yaw - aimbotAngles.yaw) + abs(ViewAngles.pitch - aimbotAngles.pitch)) {
                prevAngles = aimbotAngles;
                if (bBT) {
                    entsim = *(float*)(ent + m_flSimulationTime);
                    if (cmd->tickCount % 16 == 0) {
                        backtrack.tick = (entsim) / perTick;
                        backtrack.magnitude = magnitude;
                        backtrack.position = entHPos;
                    }
                }
            }
        }
    }
    if (GetAsyncKeyState(0x43)) {
        SlowWalk(cmd, forwardSpeed, sideSpeed);
    }
    cmd->buttons |= IN_BULLRUSH;
    if(bAA && *(int*)(localPlayer + m_iHealth) > 0) { AntiAim::desync::jitter(cmd, prevAngles, !(flags & FL_ONGROUND)); } // ANTIAIM
    else {
        *SendPacket = 1;
    }
    if (bMenuOpen && (cmd->buttons & IN_ATTACK)) {
        cmd->buttons &= ~IN_ATTACK;
    }
    if (cmd->buttons & IN_ATTACK || cmd->buttons & IN_ATTACK2 || cmd->buttons & IN_USE) {
        cmd->viewangles.pitch = clamp89(ViewAngles.pitch);
        cmd->viewangles.yaw = clamp180(ViewAngles.yaw);
    }
    if ((GetAsyncKeyState(VK_XBUTTON2)) && sqrt(pow((ViewAngles.yaw - prevAngles.yaw), 2) + pow((ViewAngles.pitch - prevAngles.pitch), 2)) <= fovAimbot && bAimbot == true) {
        if (wepEntity != NULL) {
            if ((*(float*)(wepEntity + m_flNextPrimaryAttack)) <= (float)*(int*)(localPlayer + m_nTickBase) * perTick) {
                if (bAA) {
                    btTick = backtrack.tick;
                    btMagnitude = backtrack.magnitude;
                    btEntPos = backtrack.position;
                    if (!bBT) {
                        cmd->viewangles.pitch = clamp89(prevAngles.pitch);
                        cmd->viewangles.yaw = clamp180(prevAngles.yaw);
                        cmd->buttons |= IN_ATTACK;
                    }
                    if (bBT) {
                        cmd->tickCount = btTick + 1;
                        cmd->viewangles.pitch = (180.f * (-atan((btEntPos.z - PlayerPos.z) / (btMagnitude))) / PI) - (2.f * punchAngle.pitch);
                        cmd->viewangles.yaw = 180.f * (atan2(btEntPos.y - PlayerPos.y, btEntPos.x - PlayerPos.x)) / PI - (2.f * punchAngle.yaw);
                        cmd->buttons |= IN_ATTACK;
                    }
                }
                if(!bAA) {
                    cmd->viewangles.pitch = clamp89(prevAngles.pitch);
                    cmd->viewangles.yaw = clamp180(prevAngles.yaw);
                    cmd->buttons |= IN_ATTACK;
                    *SendPacket = 0;
                }
                if(!bBT) cmd->tickCount = (entsim)/ perTick;
            }
            else if((*(float*)(wepEntity + m_flNextPrimaryAttack)) > (float)*(int*)(localPlayer + m_nTickBase) * perTick) {
                cmd->buttons &= ~IN_ATTACK; // "hide shot" 
            }
        }
    }
    if (cmd->buttons & IN_ATTACK || cmd->buttons & IN_ATTACK2) {
        bInAttack = 1;
    }
    else {
        bInAttack = 0;
    }
    clamp89(cmd->viewangles.pitch);
    clamp180(cmd->viewangles.yaw); // prevent untrusted 
    prevAngles.yaw = 18000.f;
    if (cmd->buttons & IN_ATTACK && bAA) *SendPacket = 1;
    if (*SendPacket) { // yes yes. we check if send packet or not and acccordingly setup our player rendering
        cmdView.pitch = cmd->viewangles.pitch;
        cmdView.yaw = cmd->viewangles.yaw;
    }
    if (bTP == true) {
        *(float*)(localPlayer + 0x31E8) = cmdView.pitch; // this is to view our player in thirdperson (hardcoded offset cancer)
        *(float*)(localPlayer + 0x31EC) = cmdView.yaw;
    }
    FixMovement(cmd, EngineClient, ViewAngles); // if this is removed we cannot move where we are looking
    if(!bBT) cmd->tickCount += lerp + netchan->GetLatency(FLOW_INCOMING | FLOW_OUTGOING) / perTick; // lag comp :D
    if (GetAsyncKeyState(0x58)) cmd->commandNumber = INT_MAX;
    return false;
}

void __fastcall hkOverrideView(void* ecx, void* edx, CViewSetup* pSetup) {
    DWORD localPlayer = *(DWORD*)(client + dwLocalPlayer);
    if (!localPlayer) {
        fOverrideView(ecx, edx, pSetup);
        return;
    }
    pSetup->flFOV = gFov;
    QAngle ViewAngles = { 0,0,0 };
    EngineClient->GetViewAngles(ViewAngles);
    if (*(int*)(localPlayer + m_iHealth) <= 0) {
        input->m_fCameraInThirdPerson = 0;
        fOverrideView(ecx, edx, pSetup);
        return;
    }
    input->m_fCameraInThirdPerson = bTP;
    if (bTP == true) {
        input->m_vecCameraOffset.x = ViewAngles.pitch;
        input->m_vecCameraOffset.y = ViewAngles.yaw;
        input->m_vecCameraOffset.z = 72.5f;
    }
    fOverrideView(ecx, edx, pSetup);
}

void __fastcall hkLockCursor(void* ecx, void* edx) {
    if (bMenuOpen) {
        surface->UnlockCursor();
    }
    else
    {
        fLockCursor(ecx,edx);
    }
}

void __cdecl hkSendMove() {
    fCL_SendMove();
}

bool __fastcall hkWriteUsercmdDelta(void* ecx, void* edx, int nSlot, void* buf) {
    return fWriteUsercmdDelta(ecx, edx, nSlot, buf);
}

void __fastcall hkPaint(void* ecx, void* edx, PaintMode_t mode) {
    static bool bInit = 0;
    if (bInit == 0) fontInit(Tahoma, "Tahoma", bInit);
    int screenWidth;
    int screenHeight;
    EngineClient->GetScreenSize(screenWidth, screenHeight);
    if (GetAsyncKeyState(0x5A) & 1) fLoadSkybox("sky_lunacy");
    if(mode)startDrawing(surface);
    if (mode & PAINT_INGAMEPANELS) {
        if (bBhop) drawText(Tahoma, 5, 350, (const wchar_t*)L"BHOP", green, 24, "Tahoma"); else { drawText(Tahoma, 5, 350, (const wchar_t*)L"BHOP", red, 24, "Tahoma"); }
        if (bEsp) drawText(Tahoma, 5, 370, (const wchar_t*)L"ESP", green, 24, "Tahoma"); else { drawText(Tahoma, 5, 370, (const wchar_t*)L"ESP", red, 24, "Tahoma"); }
        if (bBT) drawText(Tahoma, 5, 390, (const wchar_t*)L"BACKTRACK", green, 24, "Tahoma"); else { drawText(Tahoma, 5, 390, (const wchar_t*)L"BACKTRACK", red, 24, "Tahoma"); }
        if (bAimbot) drawText(Tahoma, 5, 410, (const wchar_t*)L"AIMBOT", green, 24, "Tahoma"); else { drawText(Tahoma, 5, 410, (const wchar_t*)L"AIMBOT", red, 24, "Tahoma"); }
        if (bAA) drawText(Tahoma, 5, 430, (const wchar_t*)L"ANTI-AIM", green, 24, "Tahoma"); else { drawText(Tahoma, 5, 430, (const wchar_t*)L"ANTI-AIM", red, 24, "Tahoma"); }
    }
    DrawMenu(mode);
    finishDrawing(surface);
    fPaint(ecx, edx, mode);
}

