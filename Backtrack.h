#pragma once
#include "includes.h"

struct btRecord
{
	int tick;
	float magnitude;
	Vector3 position;
};

btRecord sBacktrack[11];
int bestTarget = 0;
void Backtrack(CUserCmd* cmd, btRecord backtrack, int index, Vector3 PlayerPos, QAngle viewAngles, QAngle punchAngle, bool bWasAimbot) {
	static QAngle oldAngles = { 0,0,0 };
	sBacktrack[index] = backtrack;
	for (int i = 0; i < 11; i++) {
		if (&sBacktrack[i]) {
			if (sBacktrack[index].tick - sBacktrack[(index + i) % 12].tick > TIME_TO_TICKS(0.2f)) {
				sBacktrack[(index + i) % 12] = sBacktrack[index];
			}
			if (IsCloser(oldAngles, CalcAngle(PlayerPos, sBacktrack[i].position, sBacktrack[i].magnitude, punchAngle), viewAngles)) {
				oldAngles = CalcAngle(PlayerPos, sBacktrack[i].position, sBacktrack[i].magnitude, punchAngle);
				bestTarget = i;
			}
		}
	}
	cmd->tickCount = sBacktrack[bestTarget].tick;
	if (bWasAimbot == 1 && AngleIsWithin(viewAngles, oldAngles, 15.f)) {
		cmd->viewangles.pitch = CalcAngle(PlayerPos, sBacktrack[bestTarget].position, sBacktrack[bestTarget].magnitude, punchAngle).pitch;
		cmd->viewangles.yaw = CalcAngle(PlayerPos, sBacktrack[bestTarget].position, sBacktrack[bestTarget].magnitude, punchAngle).yaw;
		cmd->buttons |= IN_ATTACK;
	}
	oldAngles.yaw = 1800000.f;
}