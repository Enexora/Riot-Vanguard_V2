#pragma once
#include "Includes.h"



extern bool bAimbot = 0; // aimbot
extern bool bAA = 0; // anti aim
extern bool bBhop = 0; // bhop
extern bool bEsp = 0; // esp
extern bool bTP = 0; // third person
extern bool bBT = 0; // backtrack
bool bMenuOpen = 0;

void toggle(bool& a, int k) {
    if (GetAsyncKeyState(k) & 1) {
        a = !a;
    }
}

void toggleAll() {
    bAimbot = 0;
    bAA = 0;
    bBhop = 0;
    bEsp = 0;
    bTP = 0;
    bBT = 0;
};