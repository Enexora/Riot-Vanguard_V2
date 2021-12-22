#pragma once

#define PI 3.14159265358979323846

#define PI_F        ((float)(PI))    // Shouldn't collide with anything.
#define DEG2RAD( x )  ( (float)(x) * (float)(PI_F / 180.f) )
#define RAD2DEG( x )  ( (float)(x) * (float)(180.f / PI_F) )
#define lerp 0.03125f
#define NOMINMAX 

#include <windows.h>
#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <vector>
#include "sdk/sdk.h"
#include "csgo.hpp"

IEngineClient* EngineClient = (IEngineClient*)GetInterface("engine.dll", "VEngineClient014");
IEngineVGui* engineVGui = (IEngineVGui*)GetInterface("engine.dll", "VEngineVGui001");
IBaseClientDLL* pClientDLL = (IBaseClientDLL*)GetInterface("client.dll", "VClient018");
ISurface* surface = (ISurface*)GetInterface("vguimatsurface.dll", "VGUI_Surface031");
Player* plocalPlayer;
DWORD client;
DWORD engine;
DWORD vguimatsurface;
DWORD ClientState;
ClientModeShared* clientMode;
INetChannelInfo* netchan;
CInput* input;
DWORD localPlayer;
CGlobals* Globals;
byte* SendPacket;
bool bInAttack = 0;
vgui::Color red = { 255, 0, 0, 255 };
vgui::Color green = { 0, 230, 0, 255 };
vgui::Color blue = { 0, 0, 255, 255 };
vgui::Color white = { 255, 255, 255, 255 };
vgui::Color dark_gray = { 55,55,55, 240 };
vgui::Color black = { 0, 0, 0, 255 };
vgui::HFont HFIndicators;
vgui::HFont HFMenuTitle;
vgui::HFont HFMenuSubsections;
vgui::HFont HFMenuSliders;
vgui::HCursor cursor;
float gFov = 100.f;

#include "sdk/patternscanningIDA.h"
#include "antiaim.hpp"
#include "toggles.h"
#include "hooks.cpp"