﻿#include "includes.h"
#define TICK_INTERVAL        ( Globals->interval_per_tick ) // replace with globals->tickinterval
#define TIME_TO_TICKS( dt )    ( (int)( 0.5f + (float)(dt) / TICK_INTERVAL) )
#define TICKS_TO_TIME( t )    ( TICK_INTERVAL * ( t ) )

struct Vector3 {
	float x;
	float y;
	float z;
};

struct QAngle {
	float pitch;
	float yaw;
	float roll;
};

float clamp89(float a) {
	if (a < -89.f) {
		a = -89.f;
	}
	if (a > 89.f) {
		a = 89.f;
	}
	return a;
}
float clamp180(float a) {
	while (a < -180.f) {
		a += 360.f;
	}
	while (a > 180.f) {
		a -= 360.f;
	}
	return a;
}

float clamp450(float a) {
	if (a < -450.f) {
		a = -450.f;
	}
	if (a > 450.f) {
		a = 450.f;
	}
	return a;
}

void VectorAngles(const Vector3& forward, Vector3& angles)
{
	float tmp, yaw, pitch;

	if (forward.y == 0 && forward.x == 0)
	{
		yaw = 0;
		if (forward.z > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2f(forward.y, forward.x) * 180.f / PI_F);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrtf(forward.x * forward.x + forward.y * forward.y);
		pitch = (atan2f(-forward.z, tmp) * 180.f / PI_F);
		if (pitch < 0)
			pitch += 360;
	}

	angles.x = pitch;
	angles.y = yaw;
	angles.z = 0;
}

QAngle CalcAngle(Vector3 src, Vector3 dst, float magnitude, QAngle punchAngle) {
	QAngle ret;
	ret.pitch = (180.f * (-atan((dst.z - src.z) / (magnitude))) / PI) - (2.f * punchAngle.pitch);
	ret.yaw = 180.f * (atan2(dst.y - src.y, dst.x - src.x)) / PI - (2.f * punchAngle.yaw);
	return ret;
}

bool IsCloser(QAngle newAng, QAngle oldAng, QAngle ViewAngles) {
	 return abs(ViewAngles.yaw - newAng.yaw) + abs(ViewAngles.pitch - newAng.pitch) > abs(ViewAngles.yaw - oldAng.yaw) + abs(ViewAngles.pitch - oldAng.pitch);
}

bool InRange(int a, int min, int max) {
	return (a <= max && a >= min);
}