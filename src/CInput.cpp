#include <cstring>
#include <iostream>

#include "CInput.h"

CInput::CInput()
{
	CurrentKeyboardState = SDL_GetKeyboardState(&KeyboardStateSize);

	LastKeyboardState = new Uint8[KeyboardStateSize];
}

CInput::~CInput()
{
	delete[] LastKeyboardState;
}

void CInput::Reset()
{
	memset(&Mouse, 0, sizeof(Mouse));
}

bool CInput::Update()
{
	SDL_Event event;

	Reset();

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_MOUSEMOTION:
			{
				Mouse.Dx += event.motion.xrel;
				Mouse.Dy += event.motion.yrel;
				break;
			}

			case SDL_MOUSEBUTTONDOWN:
			{
				switch (event.button.button)
				{
					case (SDL_BUTTON_LEFT):
					{
						Mouse.LeftButton.Down = true;
						break;
					}

					case (SDL_BUTTON_MIDDLE):
					{
						Mouse.MiddleButton.Down = true;
						break;
					}

					case (SDL_BUTTON_RIGHT):
					{
						Mouse.RightButton.Down = true;
						break;
					}
				}
			}

			case SDL_MOUSEBUTTONUP:
			{
				switch (event.button.button)
				{
					case (SDL_BUTTON_LEFT):
					{
						Mouse.LeftButton.Up = true;
						break;
					}

					case (SDL_BUTTON_MIDDLE):
					{
						Mouse.MiddleButton.Up = true;
						break;
					}

					case (SDL_BUTTON_RIGHT):
					{
						Mouse.RightButton.Up = true;
						break;
					}
				}
			}
		}
	}

	Uint32 buttonsState = SDL_GetMouseState(&Mouse.X, &Mouse.Y);

	Mouse.LeftButton.Pressed = buttonsState & SDL_BUTTON(SDL_BUTTON_LEFT);
	Mouse.MiddleButton.Pressed = buttonsState & SDL_BUTTON(SDL_BUTTON_MIDDLE);
	Mouse.RightButton.Pressed = buttonsState & SDL_BUTTON(SDL_BUTTON_RIGHT);

	memcpy(LastKeyboardState, CurrentKeyboardState, KeyboardStateSize);
	SDL_GetKeyboardState(nullptr);

	return true;
}

bool CInput::IsKeyDown(Uint32 scancode)
{
	return (!LastKeyboardState[scancode] && CurrentKeyboardState[scancode]);
}

bool CInput::IsKeyPressed(Uint32 scancode)
{
	return CurrentKeyboardState[scancode];
}

bool CInput::IsKeyUp(Uint32 scancode)
{
	return (LastKeyboardState[scancode] && !CurrentKeyboardState[scancode]);
}
