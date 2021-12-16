#pragma once
#pragma once
#include "Includes.h"
#ifndef M_PI
#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h
#endif

#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.
#ifndef RAD2DEG
#define RAD2DEG( x )  ( (float)(x) * (float)(180.f / M_PI_F) )
#endif

#ifndef DEG2RAD
#define DEG2RAD( x )  ( (float)(x) * (float)(M_PI_F / 180.f) )
#endif

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
		yaw = (atan2f(forward.y, forward.x) * 180 / M_PI);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrtf(forward.x * forward.x + forward.y * forward.y);
		pitch = (atan2f(-forward.z, tmp) * 180 / M_PI);
		if (pitch < 0)
			pitch += 360;
	}

	angles.x = pitch;
	angles.y = yaw;
	angles.z = 0;
}

	void SinCos(float radians, float* sine, float* cosine);
	void AngleVectors(const Vector3& angles, Vector3* forward = nullptr, Vector3* right = nullptr, Vector3* up = nullptr);

	float DotProduct(const float* a, const float* b);

	inline float NormalizePitch(float pitch)
	{
		while (pitch > 89.f)
			pitch -= 180.f;
		while (pitch < -89.f)
			pitch += 180.f;

		return pitch;
	}

	inline float NormalizeYaw(float yaw)
	{
		if (yaw > 180)
			yaw -= (round(yaw / 360) * 360.f);
		else if (yaw < -180)
			yaw += (round(yaw / 360) * -360.f);

		return yaw;
	}


	inline float GetYawDelta(float yaw1, float yaw2)
	{
		return fabs(NormalizeYaw(yaw1 - yaw2));
	}
