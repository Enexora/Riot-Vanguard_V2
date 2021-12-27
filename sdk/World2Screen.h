#pragma once
#include "Math Structs.h"
#include "sdk.h"

bool WorldToScreen(const Vector3& origin, Vector3& screen) {
	return debugOverlay->ScreenPosition(origin, screen);

	//const VMatrix& matrix = EngineClient->WorldToScreenMatrix();

	//screen.x = matrix[0][0] * origin.x + matrix[0][1] * origin.y + matrix[0][2] * origin.y + matrix[0][3];
	//screen.y = matrix[1][0] * origin.x + matrix[1][1] * origin.y + matrix[1][2] * origin.y + matrix[1][3];

	//float w = matrix[3][0] * origin.x + matrix[3][1] * origin.y + matrix[3][2] * origin.y + matrix[3][3];

	//if (w < 0.01f)
	//	return false;
	//screen.x /= w;
	//screen.y /= w;
	//int screenWidth, screenHeight;
	//EngineClient->GetScreenSize(screenWidth, screenHeight);
	//float x = screenWidth / 2;
	//float y = screenHeight / 2;

	//x += 0.5 * screen.x * screenWidth + 0.5;
	//y -= 0.5 * screen.y * screenHeight + 0.5;
	//screen.x = x;
	//screen.y = y;
	//return true;
}
