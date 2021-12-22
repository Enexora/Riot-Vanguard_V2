
#define TICK_INTERVAL        ( 1/64 )
#define TIME_TO_TICKS( dt )    ( (int)( (float)(dt) / TICK_INTERVAL ) )
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

bool InRange(int a, int min, int max) {
	return (a <= max && a >= min);
}