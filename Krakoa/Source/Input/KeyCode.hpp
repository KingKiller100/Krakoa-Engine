#pragma once

#include <cstdint>

// Based on GLFW key codes

namespace krakoa::input
{
	enum KeyCode : std::uint16_t
	{
		KEY_SPACE = 32,
		KEY_APOSTROPHE = 39, /* ' */
		KEY_COMMA = 44, /* , */
		KEY_MINUS = 45, /* - */
		KEY_PERIOD = 46, /* . */
		KEY_SLASH = 47, /* / */
		KEY_0 = 48,
		KEY_1 = 49,
		KEY_2 = 50,
		KEY_3 = 51,
		KEY_4 = 52,
		KEY_5 = 53,
		KEY_6 = 54,
		KEY_7 = 55,
		KEY_8 = 56,
		KEY_9 = 57,
		KEY_SEMICOLON = 59, /* ; */
		KEY_EQUAL = 61, /* = */
		KEY_A = 65,
		KEY_B = 66,
		KEY_C = 67,
		KEY_D = 68,
		KEY_E = 69,
		KEY_F = 70,
		KEY_G = 71,
		KEY_H = 72,
		KEY_I = 73,
		KEY_J = 74,
		KEY_K = 75,
		KEY_L = 76,
		KEY_M = 77,
		KEY_N = 78,
		KEY_O = 79,
		KEY_P = 80,
		KEY_Q = 81,
		KEY_R = 82,
		KEY_S = 83,
		KEY_T = 84,
		KEY_U = 85,
		KEY_V = 86,
		KEY_W = 87,
		KEY_X = 88,
		KEY_Y = 89,
		KEY_Z = 90,
		KEY_LEFT_BRACKET = 91, /* [ */
		KEY_BACKSLASH = 92, /* \ */
		KEY_RIGHT_BRACKET = 93, /* ] */
		KEY_GRAVE_ACCENT = 96, /* ` */
		KEY_WORLD_1 = 161, /* non-US #1 */
		KEY_WORLD_2 = 162, /* non-US #2 */

		/* Function keys */
		KEY_ESCAPE = 256,
		KEY_ENTER = 257,
		KEY_TAB = 258,
		KEY_BACKSPACE = 259,
		KEY_INSERT = 260,
		KEY_DELETE = 261,
		KEY_RIGHT = 262,
		KEY_LEFT = 263,
		KEY_DOWN = 264,
		KEY_UP = 265,
		KEY_PAGE_UP = 266,
		KEY_PAGE_DOWN = 267,
		KEY_HOME = 268,
		KEY_END = 269,
		KEY_CAPS_LOCK = 280,
		KEY_SCROLL_LOCK = 281,
		KEY_NUM_LOCK = 282,
		KEY_PRINT_SCREEN = 283,
		KEY_PAUSE = 284,
		KEY_F1 = 290,
		KEY_F2 = 291,
		KEY_F3 = 292,
		KEY_F4 = 293,
		KEY_F5 = 294,
		KEY_F6 = 295,
		KEY_F7 = 296,
		KEY_F8 = 297,
		KEY_F9 = 298,
		KEY_F10 = 299,
		KEY_F11 = 300,
		KEY_F12 = 301,
		KEY_F13 = 302,
		KEY_F14 = 303,
		KEY_F15 = 304,
		KEY_F16 = 305,
		KEY_F17 = 306,
		KEY_F18 = 307,
		KEY_F19 = 308,
		KEY_F20 = 309,
		KEY_F21 = 310,
		KEY_F22 = 311,
		KEY_F23 = 312,
		KEY_F24 = 313,
		KEY_F25 = 314,
		KEY_KP_0 = 320,
		KEY_KP_1 = 321,
		KEY_KP_2 = 322,
		KEY_KP_3 = 323,
		KEY_KP_4 = 324,
		KEY_KP_5 = 325,
		KEY_KP_6 = 326,
		KEY_KP_7 = 327,
		KEY_KP_8 = 328,
		KEY_KP_9 = 329,
		KEY_KP_DECIMAL = 330,
		KEY_KP_DIVIDE = 331,
		KEY_KP_MULTIPLY = 332,
		KEY_KP_SUBTRACT = 333,
		KEY_KP_ADD = 334,
		KEY_KP_ENTER = 335,
		KEY_KP_EQUAL = 336,
		KEY_LEFT_SHIFT = 340,
		KEY_LEFT_CONTROL = 341,
		KEY_LEFT_ALT = 342,
		KEY_LEFT_SUPER = 343,
		KEY_RIGHT_SHIFT = 344,
		KEY_RIGHT_CONTROL = 345,
		KEY_RIGHT_ALT = 346,
		KEY_RIGHT_SUPER = 347,
		KEY_MENU = 348,

		KEY_LAST
	};
}

// #define KRK_KEY_SPACE              krakoa::input::KeyCode::KEY_SPACE
// #define KRK_KEY_APOSTROPHE         krakoa::input::KeyCode::KEY_APOSTROPHE       /* ' */
// #define KRK_KEY_COMMA              krakoa::input::KeyCode::KEY_COMMA            /* , */
// #define KRK_KEY_MINUS              krakoa::input::KeyCode::KEY_MINUS            /* - */
// #define KRK_KEY_PERIOD             krakoa::input::KeyCode::KEY_PERIOD           /* . */
// #define KRK_KEY_SLASH              krakoa::input::KeyCode::KEY_SLASH
// #define KRK_KEY_0                  krakoa::input::KeyCode::KEY_0
// #define KRK_KEY_1                  krakoa::input::KeyCode::KEY_1
// #define KRK_KEY_2                  krakoa::input::KeyCode::KEY_2
// #define KRK_KEY_3                  krakoa::input::KeyCode::KEY_3
// #define KRK_KEY_4                  krakoa::input::KeyCode::KEY_4
// #define KRK_KEY_5                  krakoa::input::KeyCode::KEY_5
// #define KRK_KEY_6                  krakoa::input::KeyCode::KEY_6
// #define KRK_KEY_7                  krakoa::input::KeyCode::KEY_7
// #define KRK_KEY_8                  krakoa::input::KeyCode::KEY_8
// #define KRK_KEY_9                  krakoa::input::KeyCode::KEY_9
// #define KRK_KEY_SEMICOLON          krakoa::input::KeyCode::KEY_SEMICOLON
// #define KRK_KEY_EQUAL              krakoa::input::KeyCode::KEY_EQUAL
// #define KRK_KEY_A                  krakoa::input::KeyCode::KEY_A
// #define KRK_KEY_B                  krakoa::input::KeyCode::KEY_B
// #define KRK_KEY_C                  krakoa::input::KeyCode::KEY_C
// #define KRK_KEY_D                  krakoa::input::KeyCode::KEY_D
// #define KRK_KEY_E                  krakoa::input::KeyCode::KEY_E
// #define KRK_KEY_F                  krakoa::input::KeyCode::KEY_F
// #define KRK_KEY_G                  krakoa::input::KeyCode::KEY_G
// #define KRK_KEY_H                  krakoa::input::KeyCode::KEY_H
// #define KRK_KEY_I                  krakoa::input::KeyCode::KEY_I
// #define KRK_KEY_J                  krakoa::input::KeyCode::KEY_J
// #define KRK_KEY_K                  krakoa::input::KeyCode::KEY_K
// #define KRK_KEY_L                  krakoa::input::KeyCode::KEY_L
// #define KRK_KEY_M                  krakoa::input::KeyCode::KEY_M
// #define KRK_KEY_N                  krakoa::input::KeyCode::KEY_N
// #define KRK_KEY_O                  krakoa::input::KeyCode::KEY_O
// #define KRK_KEY_P                  krakoa::input::KeyCode::KEY_P
// #define KRK_KEY_Q                  krakoa::input::KeyCode::KEY_Q
// #define KRK_KEY_R                  krakoa::input::KeyCode::KEY_R
// #define KRK_KEY_S                  krakoa::input::KeyCode::KEY_S
// #define KRK_KEY_T                  krakoa::input::KeyCode::KEY_T
// #define KRK_KEY_U                  krakoa::input::KeyCode::KEY_U
// #define KRK_KEY_V                  krakoa::input::KeyCode::KEY_V
// #define KRK_KEY_W                  krakoa::input::KeyCode::KEY_W
// #define KRK_KEY_X                  krakoa::input::KeyCode::KEY_X
// #define KRK_KEY_Y                  krakoa::input::KeyCode::KEY_Y
// #define KRK_KEY_Z                  krakoa::input::KeyCode::KEY_Z
// #define KRK_KEY_LEFT_BRACKET       krakoa::input::KeyCode::KEY_LEFT_BRACKET     
// #define KRK_KEY_BACKSLASH          krakoa::input::KeyCode::KEY_BACKSLASH        
// #define KRK_KEY_RIGHT_BRACKET      krakoa::input::KeyCode::KEY_RIGHT_BRACKET    
// #define KRK_KEY_GRAVE_ACCENT       krakoa::input::KeyCode::KEY_GRAVE_ACCENT     
// #define KRK_KEY_WORLD_1            krakoa::input::KeyCode::KEY_WORLD_1          
// #define KRK_KEY_WORLD_2            krakoa::input::KeyCode::KEY_WORLD_2          
//
// /* Function keys */
// #define KRK_KEY_ESCAPE             ::krakoa::input::KeyCode::KEY_ESCAPE
// #define KRK_KEY_ENTER              ::krakoa::input::KeyCode::KEY_ENTER
// #define KRK_KEY_TAB                ::krakoa::input::KeyCode::KEY_TAB
// #define KRK_KEY_BACKSPACE          ::krakoa::input::KeyCode::KEY_BACKSPACE
// #define KRK_KEY_INSERT             ::krakoa::input::KeyCode::KEY_INSERT
// #define KRK_KEY_DELETE             ::krakoa::input::KeyCode::KEY_DELETE
// #define KRK_KEY_RIGHT              ::krakoa::input::KeyCode::KEY_RIGHT
// #define KRK_KEY_LEFT               ::krakoa::input::KeyCode::KEY_LEFT
// #define KRK_KEY_DOWN               ::krakoa::input::KeyCode::KEY_DOWN
// #define KRK_KEY_UP                 ::krakoa::input::KeyCode::KEY_UP
// #define KRK_KEY_PAGE_UP            ::krakoa::input::KeyCode::KEY_PAGE_UP
// #define KRK_KEY_PAGE_DOWN          ::krakoa::input::KeyCode::KEY_PAGE_DOWN
// #define KRK_KEY_HOME               ::krakoa::input::KeyCode::KEY_HOME
// #define KRK_KEY_END                ::krakoa::input::KeyCode::KEY_END
// #define KRK_KEY_CAPS_LOCK          ::krakoa::input::KeyCode::KEY_CAPS_LOCK
// #define KRK_KEY_SCROLL_LOCK        ::krakoa::input::KeyCode::KEY_SCROLL_LOCK
// #define KRK_KEY_NUM_LOCK           ::krakoa::input::KeyCode::KEY_NUM_LOCK
// #define KRK_KEY_PRINT_SCREEN       ::krakoa::input::KeyCode::KEY_PRINT_SCREEN
// #define KRK_KEY_PAUSE              ::krakoa::input::KeyCode::KEY_PAUSE
// #define KRK_KEY_F1                 ::krakoa::input::KeyCode::KEY_F1
// #define KRK_KEY_F2                 ::krakoa::input::KeyCode::KEY_F2
// #define KRK_KEY_F3                 ::krakoa::input::KeyCode::KEY_F3
// #define KRK_KEY_F4                 ::krakoa::input::KeyCode::KEY_F4
// #define KRK_KEY_F5                 ::krakoa::input::KeyCode::KEY_F5
// #define KRK_KEY_F6                 ::krakoa::input::KeyCode::KEY_F6
// #define KRK_KEY_F7                 ::krakoa::input::KeyCode::KEY_F7
// #define KRK_KEY_F8                 ::krakoa::input::KeyCode::KEY_F8
// #define KRK_KEY_F9                 ::krakoa::input::KeyCode::KEY_F9
// #define KRK_KEY_F10                ::krakoa::input::KeyCode::KEY_F10
// #define KRK_KEY_F11                ::krakoa::input::KeyCode::KEY_F11
// #define KRK_KEY_F12                ::krakoa::input::KeyCode::KEY_F12
// #define KRK_KEY_F13                ::krakoa::input::KeyCode::KEY_F13
// #define KRK_KEY_F14                ::krakoa::input::KeyCode::KEY_F14
// #define KRK_KEY_F15                ::krakoa::input::KeyCode::KEY_F15
// #define KRK_KEY_F16                ::krakoa::input::KeyCode::KEY_F16
// #define KRK_KEY_F17                ::krakoa::input::KeyCode::KEY_F17
// #define KRK_KEY_F18                ::krakoa::input::KeyCode::KEY_F18
// #define KRK_KEY_F19                ::krakoa::input::KeyCode::KEY_F19
// #define KRK_KEY_F20                ::krakoa::input::KeyCode::KEY_F20
// #define KRK_KEY_F21                ::krakoa::input::KeyCode::KEY_F21
// #define KRK_KEY_F22                ::krakoa::input::KeyCode::KEY_F22
// #define KRK_KEY_F23                ::krakoa::input::KeyCode::KEY_F23
// #define KRK_KEY_F24                ::krakoa::input::KeyCode::KEY_F24
// #define KRK_KEY_F25                ::krakoa::input::KeyCode::KEY_F25
// #define KRK_KEY_KP_0               ::krakoa::input::KeyCode::KEY_KP_0
// #define KRK_KEY_KP_1               ::krakoa::input::KeyCode::KEY_KP_1
// #define KRK_KEY_KP_2               ::krakoa::input::KeyCode::KEY_KP_2
// #define KRK_KEY_KP_3               ::krakoa::input::KeyCode::KEY_KP_3
// #define KRK_KEY_KP_4               ::krakoa::input::KeyCode::KEY_KP_4
// #define KRK_KEY_KP_5               ::krakoa::input::KeyCode::KEY_KP_5
// #define KRK_KEY_KP_6               ::krakoa::input::KeyCode::KEY_KP_6
// #define KRK_KEY_KP_7               ::krakoa::input::KeyCode::KEY_KP_7
// #define KRK_KEY_KP_8               ::krakoa::input::KeyCode::KEY_KP_8
// #define KRK_KEY_KP_9               ::krakoa::input::KeyCode::KEY_KP_9
// #define KRK_KEY_KP_DECIMAL         ::krakoa::input::KeyCode::KEY_KP_DECIMAL
// #define KRK_KEY_KP_DIVIDE          ::krakoa::input::KeyCode::KEY_KP_DIVIDE
// #define KRK_KEY_KP_MULTIPLY        ::krakoa::input::KeyCode::KEY_KP_MULTIPLY
// #define KRK_KEY_KP_SUBTRACT        ::krakoa::input::KeyCode::KEY_KP_SUBTRACT
// #define KRK_KEY_KP_ADD             ::krakoa::input::KeyCode::KEY_KP_ADD
// #define KRK_KEY_KP_ENTER           ::krakoa::input::KeyCode::KEY_KP_ENTER
// #define KRK_KEY_KP_EQUAL           ::krakoa::input::KeyCode::KEY_KP_EQUAL
// #define KRK_KEY_LEFT_SHIFT         ::krakoa::input::KeyCode::KEY_LEFT_SHIFT
// #define KRK_KEY_LEFT_CONTROL       ::krakoa::input::KeyCode::KEY_LEFT_CONTROL
// #define KRK_KEY_LEFT_ALT           ::krakoa::input::KeyCode::KEY_LEFT_ALT
// #define KRK_KEY_LEFT_SUPER         ::krakoa::input::KeyCode::KEY_LEFT_SUPER
// #define KRK_KEY_RIGHT_SHIFT        ::krakoa::input::KeyCode::KEY_RIGHT_SHIFT
// #define KRK_KEY_RIGHT_CONTROL      ::krakoa::input::KeyCode::KEY_RIGHT_CONTROL
// #define KRK_KEY_RIGHT_ALT          ::krakoa::input::KeyCode::KEY_RIGHT_ALT
// #define KRK_KEY_RIGHT_SUPER        ::krakoa::input::KeyCode::KEY_RIGHT_SUPER
// #define KRK_KEY_MENU               ::krakoa::input::KeyCode::KEY_MENU
//
// #define KRK_KEY_LAST               ::krakoa::input::KeyCode::KEY_LAST
