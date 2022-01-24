#pragma once
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)

// USEAGE: KEY_DOWN(KEY_CODE)
//	KEY_CODES: @KeyCode.png
