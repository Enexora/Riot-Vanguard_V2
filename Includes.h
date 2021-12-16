#pragma once
#define PI 3.141592653
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( PI ) / 180.0f ) )
#include "windows.h"
#include <iostream>
#include <string>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include "sdk.h"
#include "csgo.hpp"

LPDIRECT3DDEVICE9 pDevice = nullptr;
DWORD client;
DWORD engine;
IEngineClient* EngineClient = (IEngineClient*)GetInterface("engine.dll", "VEngineClient014");
IEngineVGui* engineVGui = (IEngineVGui*)GetInterface("engine.dll", "VEngineVGui001");
DWORD ClientState;
void* pClientDLL = (void*)GetInterface("client.dll", "VClient018");
ISurface* surface = (ISurface*)GetInterface("vguimatsurface.dll", "VGUI_Surface031");
ClientModeShared* clientMode;
INetChannelInfo* netchan;
CInput* input;
DWORD localPlayer;
CGlobals* globals;
byte* SendPacket;
vgui::Color red = { 255, 0, 0, 255 };
vgui::Color green = { 0, 230, 0, 255 };
vgui::Color blue = { 0, 0, 255, 255 };
vgui::Color white = { 255, 255, 255, 255 };
vgui::Color dark_gray = { 55,55,55, 240 };
vgui::Color black = { 0, 0, 0, 255 };
vgui::HFont Tahoma;
vgui::HCursor cursor;

#include "AntiAim.hpp"
#include "extramath.h"
#include <math.h>
#include <TlHelp32.h>
#include "Toggles.h"
#include "hooks.cpp"
