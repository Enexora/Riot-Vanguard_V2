#pragma once
#include "menu/menu.h"


DWORD* pVTableBase = 0;
DWORD* pVTableFnc = 0;
QAngle aimbotAngles = { 0,0,0 };
QAngle prevAngles = { 0,0,0 };
float fovAimbot = 15.f;
bool flip = false;
float lastTime;

void Detour(void* src, void* dst) {
    static DWORD oldProtect;
    VirtualProtect(src, 0x5, PAGE_EXECUTE_READWRITE, &oldProtect);
    *(BYTE*)(src) = 0xE8;
    *(DWORD*)((DWORD)src + 1) = ((DWORD)dst - (DWORD)src - 5);
    VirtualProtect(src, 0x5, oldProtect, 0);
}

void FixMovement(CUserCmd* cmd, QAngle viewangles)
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

void triggerbot(CUserCmd* cmd) {
    Player* ent = *(Player**)(client + dwEntityList + (*(int*)(localPlayer + m_iCrosshairId) - 1) * 0x10);
    if (ent == localPlayer || *(int*)(ent + m_iTeamNum) == *(int*)(localPlayer + m_iTeamNum)) {
        return;
    }
    cmd->buttons |= IN_ATTACK;
}

static QAngle cmdView = { 0,0,0 };
bool __fastcall hkCreateMove(void* ecx, void* edx, float flSampleTimer, CUserCmd* cmd) {
    ClientState = *(DWORD*)(engine + dwClientState);
    static bool shouldResetbtRecords = 1;
    if (!EngineClient->IsInGame()) { return fCreateMove(ecx, edx, flSampleTimer, cmd); shouldResetbtRecords = 1; }
    static bool shotLast = 1;
    static btRecord backtrack[11] = { 0, 0, {0,0,0} };
    static int btIndex = 0;
    static int trollEnt = 0;
    localPlayer = *(Player**)(client + dwLocalPlayer);
    static int btTick = 0;
    if (localPlayer == NULL) return false;
    DWORD flags = *(int*)(localPlayer + m_fFlags);
    ViewAngles = *(QAngle*)(ClientState + dwClientState_ViewAngles);
    if (bBhop == true) {
        if (cmd->buttons & IN_JUMP && !(flags & FL_ONGROUND)) {
            cmd->buttons &= ~IN_JUMP;
        }
    }
    if (shouldResetbtRecords && EngineClient->IsInGame()) {
        resetRecords(shouldResetbtRecords, backtrack);
    }
    float sideSpeed = cmd->sidemove;
    float forwardSpeed = cmd->forwardmove;
    if (!localPlayer) return false;
    Vector3 PlayerPos = localPlayer->m_vecOrigin();
    WeaponEntity* wepEntity = *(WeaponEntity**)(client + dwEntityList + ((localPlayer->m_hActiveWeapon() & 0xFFF) - 1) * 0x10);
    static float entsim;
    static float bestMagnitude;
    static Vector3 bombAssHead;
    Vector3 entHPos;
    for (int i = 0; i < 64; i++) {                                                       // Ent list for aimbot
            Player* ent = *(Player**)(client + dwEntityList + i * 0x10);
            if (ent == NULL) {
                aimbotAngles = ViewAngles;
                continue;
            }
            entHPos = ent->m_vecOrigin();
            Vector3 entVelocity = *(Vector3*)(ent + m_vecVelocity);
            PlayerPos = localPlayer->m_vecOrigin();
            float trollmagnitude = sqrt(((entHPos.x - PlayerPos.x) * (entHPos.x - PlayerPos.x)) + ((entHPos.y - PlayerPos.y) * (entHPos.y - PlayerPos.y)));
            if(*(int*)(localPlayer + m_iCrosshairId) != 0){
                if (GetAsyncKeyState(0x54) & 1) {
                    trollEnt = *(int*)(localPlayer + m_iCrosshairId) - 1;
                }
                triggerbot(cmd);
            }
            if (trollEnt == i && EntityList->GetClientEntity(trollEnt) != localPlayer && *(int*)(*(DWORD*)(client + dwEntityList + trollEnt * 0x10) + m_iHealth) > 0 && GetAsyncKeyState(0x46)){
                cmd->forwardmove = clamp450(cos(DEG2RAD(ViewAngles.yaw)) * (250 * (ent->m_vecOrigin().x - localPlayer->m_vecOrigin().x) + entVelocity.x) + sin(DEG2RAD(ViewAngles.yaw)) * (250 * (ent->m_vecOrigin().y - localPlayer->m_vecOrigin().y) + entVelocity.y));
                cmd->sidemove = clamp450(cos(DEG2RAD(ViewAngles.yaw)) * -(250 * (ent->m_vecOrigin().y - localPlayer->m_vecOrigin().y) + entVelocity.y) + sin(DEG2RAD(ViewAngles.yaw)) * (250 * (ent->m_vecOrigin().x - localPlayer->m_vecOrigin().x) + entVelocity.x));
            }

            entHPos.x = *(float*)(*(DWORD*)(ent + m_dwBoneMatrix) + 0x30 * 8 + 0x0C);
            entHPos.y = *(float*)(*(DWORD*)(ent + m_dwBoneMatrix) + 0x30 * 8 + 0x1C);
            entHPos.z = *(float*)(*(DWORD*)(ent + m_dwBoneMatrix) + 0x30 * 8 + 0x2C);
            PlayerPos.z += localPlayer->m_vecViewOffset().z;
            if (bAimbot == true) {
            if (*(bool*)(ent + m_bDormant) == 1 || ent == localPlayer || ent->m_iTeamNum() == localPlayer->m_iTeamNum() || ent->m_iHealth() <= 0) { //checks before running actual aimbot lol
                aimbotAngles = ViewAngles;
                continue;
            }
            float magnitude = sqrt(((entHPos.x - PlayerPos.x) * (entHPos.x - PlayerPos.x)) + ((entHPos.y - PlayerPos.y) * (entHPos.y - PlayerPos.y)));
            if (magnitude == 0) {
                continue;
            }
            aimbotAngles.pitch = CalcAngle(PlayerPos, entHPos, magnitude, localPlayer->m_aimPunchAngle()).pitch;
            aimbotAngles.yaw = CalcAngle(PlayerPos, entHPos, magnitude, localPlayer->m_aimPunchAngle()).yaw;
            clamp89(aimbotAngles.pitch);
            clamp180(aimbotAngles.yaw);
            if (IsCloser(prevAngles, aimbotAngles, ViewAngles)) {
                prevAngles = aimbotAngles;
                entsim = *(float*)((DWORD)ent + m_flSimulationTime);
                bombAssHead = entHPos;
                bestMagnitude = magnitude;
            }
        }
    }
    if (bBT) {
        if (TIME_TO_TICKS(entsim) > backtrack[btIndex - 1].tick && AngleIsWithin(ViewAngles, CalcAngle(PlayerPos, bombAssHead, bestMagnitude, localPlayer->m_aimPunchAngle()), fovAimbot + 5.f)) {
            if (btIndex >= 11) btIndex = 0;
            backtrack[btIndex].tick = TIME_TO_TICKS(entsim);
            backtrack[btIndex].magnitude = bestMagnitude;
            backtrack[btIndex].position = bombAssHead;
            Backtrack(cmd, backtrack[btIndex], btIndex, PlayerPos, ViewAngles, localPlayer->m_aimPunchAngle(), 0);
            btIndex = btIndex >= 11 ? 0 : btIndex + 1;
        }
    }
    if (GetAsyncKeyState(0x43)) {
        SlowWalk(cmd, forwardSpeed, sideSpeed);
    }
    if (bAA && localPlayer->m_iHealth() > 0) AntiAim::desync::helicopterFast(cmd, prevAngles, !(flags & FL_ONGROUND)); // ANTIAIM
    else {
        *SendPacket = 1;
    }
    if (GetAsyncKeyState(VK_NUMPAD5)) {
        using tSetClanTag = void(__fastcall*)(const char*, const char*);
        static tSetClanTag SetClanTag = (tSetClanTag)(engine + dwSetClanTag);
        SetClanTag(u8"⌠Vanguard⌡", "R Vanguard");
    }

    if (bMenuOpen && (cmd->buttons & IN_ATTACK)) {
        cmd->buttons &= ~IN_ATTACK;
    }
    if (wepEntity) {
        if (wepEntity->m_flNextPrimaryAttack() > TICKS_TO_TIME(localPlayer->m_nTickBase())) {
            cmd->buttons &= ~IN_ATTACK;
        }
    }
    if (cmd->buttons & IN_ATTACK || cmd->buttons & IN_USE) {
        cmd->viewangles.pitch = clamp89(ViewAngles.pitch);
        cmd->viewangles.yaw = clamp180(ViewAngles.yaw);
        if (bBT && localPlayer->m_iHealth() > 0 && &sBacktrack[bestTarget]) {
            cmd->tickCount = sBacktrack[bestTarget].tick;
        }
    }
    if (wepEntity) {
        if (cmd->buttons & IN_ATTACK2 && wepEntity->m_iItemDefinitionIndex() == WEAPON_KNIFE || wepEntity->m_iItemDefinitionIndex() == WEAPON_REVOLVER) {
            cmd->viewangles.pitch = clamp89(ViewAngles.pitch);
            cmd->viewangles.yaw = clamp180(ViewAngles.yaw);
            if (bBT && localPlayer->m_iHealth() > 0 && &sBacktrack[bestTarget]) {
                cmd->tickCount = sBacktrack[bestTarget].tick;
            }
        }
    }
    if ((GetAsyncKeyState(VK_XBUTTON2)) && (AngleIsWithin(ViewAngles, prevAngles, fovAimbot) || AngleIsWithin(ViewAngles, CalcAngle(PlayerPos, sBacktrack[bestTarget].position, bestMagnitude, localPlayer->m_aimPunchAngle()), fovAimbot)) && bAimbot == true) {
        if (wepEntity != NULL) {
            if (wepEntity->m_flNextPrimaryAttack() <= TICKS_TO_TIME(localPlayer->m_nTickBase()) || wepEntity->m_iBurstShotsRemaining() > 0) {
                if (bAA) {
                    if (!bBT) {
                        cmd->viewangles.pitch = clamp89(prevAngles.pitch);
                        cmd->viewangles.yaw = clamp180(prevAngles.yaw);
                        cmd->buttons |= IN_ATTACK;
                    }
                }
                if(!bAA) {
                    cmd->viewangles.pitch = clamp89(prevAngles.pitch);
                    cmd->viewangles.yaw = clamp180(prevAngles.yaw);
                    cmd->buttons |= IN_ATTACK;
                    *SendPacket = 0;
                }
                if (bBT) {
                    Backtrack(cmd, backtrack[btIndex], btIndex, PlayerPos, ViewAngles, localPlayer->m_aimPunchAngle(), 1);
                    if (AngleIsWithin(ViewAngles, CalcAngle(PlayerPos, sBacktrack[bestTarget].position, sBacktrack[bestTarget].magnitude, localPlayer->m_aimPunchAngle()), fovAimbot)) {
                        cmd->viewangles.pitch = CalcAngle(PlayerPos, sBacktrack[bestTarget].position, sBacktrack[bestTarget].magnitude, localPlayer->m_aimPunchAngle()).pitch;
                        cmd->viewangles.yaw = CalcAngle(PlayerPos, sBacktrack[bestTarget].position, sBacktrack[bestTarget].magnitude, localPlayer->m_aimPunchAngle()).yaw;
                        cmd->buttons |= IN_ATTACK;
                    }
                    cmd->tickCount = sBacktrack[bestTarget].tick;
                }
            }
        }
    }
    clamp89(cmd->viewangles.pitch);
    clamp180(cmd->viewangles.yaw); // prevent untrusted 
    prevAngles.yaw = 18000.f;
    if (!bBT) cmd->tickCount += TIME_TO_TICKS(netchan->GetLatency(FLOW_OUTGOING));
    if (cmd->buttons & IN_ATTACK && bAA) *SendPacket = 1;
    if (*SendPacket) { // yes yes. we check if send packet or not and acccordingly setup our player rendering
        cmdView.pitch = cmd->viewangles.pitch;
        cmdView.yaw = cmd->viewangles.yaw;
    }
    if (bTP == true) {
        localPlayer->getLocalBullshitVisualAngle()->pitch = cmdView.pitch;
        localPlayer->getLocalBullshitVisualAngle()->yaw = cmdView.yaw;
    }
    FixMovement(cmd, ViewAngles); // if this is removed we cannot move where we are looking
    if (GetAsyncKeyState(0x58)) cmd->commandNumber = INT_MAX;
    return false;
}

void __fastcall hkOverrideView(void* ecx, void* edx, CViewSetup* pSetup) {
    if (!localPlayer) {
        fOverrideView(ecx, edx, pSetup);
        return;
    }
    WeaponEntity* wepEntity = *(WeaponEntity**)(client + dwEntityList + ((localPlayer->m_hActiveWeapon() & 0xFFF) - 1) * 0x10);
    if (!wepEntity) {
        fOverrideView(ecx, edx, pSetup);
        return;
    }
    pSetup->flFOV = gFov;
    if (wepEntity->m_zoomLevel() == 2 && wepEntity->isSniper() == true) {
        if (wepEntity->m_flNextPrimaryAttack() <= TICKS_TO_TIME(localPlayer->m_nTickBase())) {
            pSetup->flFOV = gFov / 2;
        }
        if (wepEntity->m_iItemDefinitionIndex() == WEAPON_SCAR20 || wepEntity->m_iItemDefinitionIndex() == WEAPON_G3SG1) {
            pSetup->flFOV = gFov / 2;
        }
    }
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

void __fastcall hkFrameStageNotify(void* ecx, void* edx, int stage) {
    fFrameStageNotify(ecx, edx, stage);
}

void __fastcall hkPaint(void* ecx, void* edx, PaintMode_t mode) {
    static bool bInit = 0;
    WeaponEntity* wepEntity = nullptr;
    if (localPlayer) wepEntity = *(WeaponEntity**)(client + dwEntityList + ((localPlayer->m_hActiveWeapon() & 0xFFF) - 1) * 0x10);
    if (bInit == 0) {
        fontInit(HFIndicators, "Tahoma", bInit);
        fontInit(HFMenuTitle, "Consolas", bInit);
        fontInit(HFMenuSubsections, "Consolas", bInit);
        fontInit(HFMenuSliders, "Consolas", bInit);
    }
    int screenWidth;
    int screenHeight;
    EngineClient->GetScreenSize(screenWidth, screenHeight);
    startDrawing(surface);
    if (GetAsyncKeyState(0x5A) & 1) {
        fLoadSkybox("sky_lunacy");
    }
    if (mode & PAINT_UIPANELS) {
        DrawMenu();
        if (EngineClient->IsInGame()) DrawIndicators();
        if (wepEntity) {
            if (wepEntity->m_zoomLevel() != 0 && wepEntity->isSniper()) {
                if (wepEntity->m_flNextPrimaryAttack() <= TICKS_TO_TIME(localPlayer->m_nTickBase()) || wepEntity->m_iItemDefinitionIndex() == WEAPON_SCAR20 || wepEntity->m_iItemDefinitionIndex() == WEAPON_G3SG1) {
                    surface->SetDrawColor(black);
                    surface->DrawLine(0, screenHeight / 2, screenWidth, screenHeight / 2);
                    surface->DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight);
                }
            }
        }
    }
    if (!wepEntity) {
        fPaint(ecx, edx, mode);
        finishDrawing(surface);
        return;
    }
    if (wepEntity->m_zoomLevel() != 1 && mode & PAINT_UIPANELS) {
        fPaint(ecx, edx, mode);
        finishDrawing(surface);
    }
}
