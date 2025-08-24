#pragma once

/*
Definitions
*/
#define WIN_WIDTH 1000u									// Drawing window width in pixels
#define WIN_HEIGHT 600u									// Drawing window height in pixels

#define DEFAULT_ORIGIN_X WIN_WIDTH / 2.f				// Default X coordinates where the 3D origin will appear in the screen
#define DEFAULT_ORIGIN_Y -(WIN_HEIGHT / 2.f)			// Default Y coordinates where the 3D origin will appear in the screen

#define POINTS_FNAME "points.pts"						// File from which the points will be read (to avoid having to enter it every time the program is opened)

#define FPS 120u										// Maximum frames per second that will be rendered
#define MS_PER_FRAME (1000/FPS)							// Time (in ms) for each frame

#define BG_COLOR 0x10, 0x10, 0x10, SDL_ALPHA_OPAQUE		// Default background color (in RGB format)

#define DEFAULT_CAM_ZVALUE -600.f						// Default Z coordinates of the camera's position
#define FOV_Y_DEG 10.f									// Camera's field of view (in degrees)

#define ANGLE_STEP_DEG 1.f								// Amount (in degrees) that the shape will be rotated in the specified direction for every frame with button press

#define TO_RAD_CONSTANT 3.141592 / 180.0				// Multiply by this to convert from deg to rad