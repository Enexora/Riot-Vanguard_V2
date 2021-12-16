#pragma once
#include "Includes.h"


namespace AntiAim {
    namespace desync {
        void Static(CUserCmd* cmd, QAngle prevAngles, bool inair) {
            int chokeCount = 6;
            if (inair == true) chokeCount = 15;
            if (EngineClient->IsVoiceRecording()) { chokeCount = 4; }
            *SendPacket = (*(int*)(clientstate_choked_commands + ClientState) >= chokeCount);
            cmd->viewangles.pitch = 89.f;
            static float flstored_yaw;
            if (*SendPacket == 1) {
                cmd->viewangles.yaw = clamp180(cmd->viewangles.yaw + 90.f);
                flstored_yaw = cmd->viewangles.yaw;
            }
            if (*SendPacket == 0) {
                cmd->viewangles.yaw = clamp180(flstored_yaw + 120.f);
                if (cmd->buttons & IN_DUCK) {
                    cmd->sidemove += cmd->tickCount % 2 ? 3.25 : -3.25;
                }
                if (cmd->buttons & ~IN_DUCK) {
                    cmd->sidemove += cmd->tickCount % 2 ? 1.01 : -1.01;
                }
            }
            cmd->tickCount += 1;
        }

        void jitter(CUserCmd* cmd, QAngle prevAngles, bool inair) {
            int chokeCount = 13;
            static int sentPackets = 0;
            if (inair == true) chokeCount = 15;
            Vector3 velocity = *(Vector3*)(localPlayer + m_vecVelocity);
            if (sentPackets == INT_MAX) sentPackets = 0;
            if(*SendPacket == 1) sentPackets += 1;
            if (EngineClient->IsVoiceRecording()) { chokeCount = 4; }
            *SendPacket = (*(int*)(clientstate_choked_commands + ClientState) >= chokeCount);
            cmd->viewangles.pitch = 89.f;
            static float flstored_yaw;
            if (*SendPacket == 1) {
                cmd->viewangles.yaw = sentPackets % 6 == 0 ? clamp180(prevAngles.yaw + 130.f + (55.8f * (1 - (velocity.x + velocity.y) / 250))) : clamp180(prevAngles.yaw - 130.f - (42.943f * (1 - (velocity.x + velocity.y) / 250)));
                flstored_yaw = cmd->viewangles.yaw;
            }
            if (*SendPacket == 0) {
                cmd->viewangles.yaw = clamp180(flstored_yaw - 120.f);
                if (cmd->buttons & IN_DUCK) {
                    cmd->sidemove += cmd->tickCount % 2 ? 3.25 : -3.25;
                }
                if (cmd->buttons & ~IN_DUCK) {
                    cmd->sidemove += cmd->tickCount % 2 ? 1.01 : -1.01;
                }
            }
            cmd->tickCount += 1;
        }

        void helicopter(CUserCmd* cmd, QAngle prevAngles, bool inair) {
            int chokeCount = 6;
            if (inair == true) chokeCount = 15;
            Vector3 velocity = *(Vector3*)(localPlayer + m_vecVelocity);
            if (EngineClient->IsVoiceRecording()) chokeCount = 4; 
            *SendPacket = (*(int*)(clientstate_choked_commands + ClientState) >= chokeCount);
            cmd->viewangles.pitch = 89.f;
            static float flstored_yaw;
            if (*SendPacket == 1) {
                cmd->viewangles.yaw = clamp180(flstored_yaw + 73.231f);
                flstored_yaw = cmd->viewangles.yaw;
            }
            if (flstored_yaw > 180) flstored_yaw -= 360;
            if (flstored_yaw < -180) flstored_yaw += 360;
            if (*SendPacket == 0) {
                cmd->viewangles.yaw = clamp180(flstored_yaw - 120.f);
                if (cmd->buttons & IN_DUCK) {
                    cmd->sidemove += cmd->tickCount % 2 ? 3.25 : -3.25;
                }
                if (cmd->buttons & ~IN_DUCK) {
                    cmd->sidemove += cmd->tickCount % 2 ? 1.01 : -1.01;
                }
            }
            cmd->tickCount += 1;
        }

        void helicopterFast(CUserCmd* cmd, QAngle prevAngles, bool inair) {
            int chokeCount = 3;
            if (inair == true) chokeCount = 15;
            Vector3 velocity = *(Vector3*)(localPlayer + m_vecVelocity);
            if (EngineClient->IsVoiceRecording() && chokeCount <= 4) chokeCount = 4;
            *SendPacket = (*(int*)(clientstate_choked_commands + ClientState) >= chokeCount);
            cmd->viewangles.pitch = 89.f;
            static float flstored_yaw;
            if (*SendPacket == 1) {
                cmd->viewangles.yaw = clamp180(flstored_yaw + 37.231f);
                flstored_yaw = cmd->viewangles.yaw;
            }
            if (flstored_yaw > 180) flstored_yaw -= 360;
            if (flstored_yaw < -180) flstored_yaw += 360;
            if (*SendPacket == 0) {
                cmd->viewangles.yaw = clamp180(flstored_yaw - 120.f);
                if (cmd->buttons & IN_DUCK) {
                    cmd->sidemove += cmd->tickCount % 2 ? 3.25 : -3.25;
                }
                if (cmd->buttons & ~IN_DUCK) {
                    cmd->sidemove += cmd->tickCount % 2 ? 1.01 : -1.01;
                }
            }
            cmd->tickCount += 1;
        }
    }
};