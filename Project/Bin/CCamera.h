//==============================================================================
#ifndef _CCAMERA_H_
#define _CCAMERA_H_

#include <SDL.h>

#include "Define.h"

//==============================================================================

enum {
	TARGET_MODE_NORMAL = 0,
	TARGET_MODE_CENTER
};

//==============================================================================
class CCamera {
public:
	static CCamera CameraControl;
	int TargetMode;

	CCamera();

	void OnMove(float MoveX, float MoveY);

	int GetX();
	int GetY();

	void SetPos(int X, int Y);

private:
	float X;
	float Y;
};

//==============================================================================

#endif
