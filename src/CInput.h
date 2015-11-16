#pragma once

#include <SDL2/SDL.h>

class CInput
{
public:
	struct
	{
		int X;
		int Y;
		int Dx;
		int Dy;

		struct
		{
			bool Down;
			bool Pressed;
			bool Up;
		} LeftButton, MiddleButton, RightButton;
	} Mouse;

	CInput();
	~CInput();

	void Reset();
	bool Update();

	bool IsKeyDown(Uint32 scancode);
	bool IsKeyPressed(Uint32 scancode);
	bool IsKeyUp(Uint32 scancode);

	int KeyboardStateSize;
	const Uint8* CurrentKeyboardState;
	Uint8* LastKeyboardState;
};