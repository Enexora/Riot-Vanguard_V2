#pragma once
#include "includes.h"

struct btRecord
{
	int tick;
	float magnitude;
	Vector3 position;
};

void Backtrack(CUserCmd* cmd, btRecord backtrack, int index, Vector3 PlayerPos, QAngle punchAngle, bool bWasAimbot) {
	QAngle ViewAngles;
	EngineClient->GetViewAngles(ViewAngles);
	static QAngle oldAngles = { 0,0,0 };
	static QAngle newAngles;
	static int bestTarget = 0;
	btRecord sBacktrack[11];
	sBacktrack[index] = backtrack;
	if (IsCloser(CalcAngle(backtrack.position, PlayerPos, backtrack.magnitude, punchAngle), oldAngles, ViewAngles)) {
		newAngles = CalcAngle(backtrack.position, PlayerPos, backtrack.magnitude, punchAngle);
		bestTarget = index;
	}
	oldAngles = newAngles;
	if(cmd->buttons & IN_ATTACK) cmd->tickCount = sBacktrack[bestTarget].tick;
	if (bWasAimbot) {
		cmd->viewangles.pitch = newAngles.pitch;
		cmd->viewangles.yaw = newAngles.yaw;
		cmd->buttons |= IN_ATTACK;
	}
}