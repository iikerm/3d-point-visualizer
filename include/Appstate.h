#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "Vector3f.h"
#include "constants.h"


typedef struct {
	bool checkMouse;				// To know when the user's mouse input should be registered
	bool showDebugInfo;				// To know if the debug information (i.e. point coords, rotation info etc.) should be shown
	bool computeTransformations;	// To ensure transformations are only computed when necessary and not in all the frames
	SDL_FPoint oldMousePos;			// To compare with the actual mouse position if needed to calculate difference in position
} InOutHandle;

/*
Returns an InOutHandle element initialized with the default values
*/
inline InOutHandle defaultInOutHandle() {
	InOutHandle ioHandle;

	ioHandle.checkMouse = false;
	ioHandle.showDebugInfo = false;
	ioHandle.computeTransformations = false;
	SDL_GetMouseState(&ioHandle.oldMousePos.x, &ioHandle.oldMousePos.y);

	return ioHandle;
}


typedef struct {
	Vector3f rotationAngles;			// Angles (in degrees) that the points have been rotated around each axis
	float zCamValue;					// Z Value for the camera (i.e. zoom)
	unsigned long nPoints;				// Number of points read from the file
	Vector3f midPoint;					// 'Average' point (i.e. point supposedly in the middle of all the points)
	SDL_FPoint originXY;				// Origin coordinates (i.e. 2D point in the screen where the (0, 0, 0) coordinate is drawn)

	Vector3f* pointsArray_3d;			// Array containing points to be drawn (in 3D), MUST BE INITIALIZED WITH AN ARRAY OF POINTS BEFORE USE
	SDL_FPoint* pointsArray;			// 2D mapping of the 3D array, MUST BE INITIALIZED WITH AN ARRAY OF POINTS BEFORE USE
} GeometryHandle;

/*
Returns a GeometryHandle element initialized with the default values. NONE OF THE POINT ARRAYS ARE INITIALIZED.
*/
inline GeometryHandle defaultGeometryHandle() {
	SDL_FPoint defaultOrigin;
	defaultOrigin.x = DEFAULT_ORIGIN_X;
	defaultOrigin.y = DEFAULT_ORIGIN_Y;

	GeometryHandle geoHandle;
	geoHandle.rotationAngles = makeVector3f(0, 0, 0);
	geoHandle.zCamValue = DEFAULT_CAM_ZVALUE;
	geoHandle.nPoints = 0ul;
	geoHandle.midPoint = makeVector3f(0, 0, 0);
	geoHandle.originXY = defaultOrigin;

	return geoHandle;
}


typedef struct {
	Vector3f origin;		// 3D coordinate that will act as the origin of all other 3D points drawn
	Vector3f xAxis;			// Arbitrary point along the X axis that is used to render said axis
	Vector3f yAxis;			// Arbitrary point along the Y axis that is used to render said axis
	Vector3f zAxis;			// Arbitrary point along the Z axis that is used to render said axis
} Axes;

/*
Returns an Axes element initialized with 100-point long axes in the x, y and z directions
*/
inline Axes defaultAxes(float axisLength) {
	Axes ax;
	ax.origin = makeVector3f(0, 0, 0);
	ax.xAxis = makeVector3f(axisLength, 0, 0);
	ax.yAxis = makeVector3f(0, axisLength, 0);
	ax.zAxis = makeVector3f(0, 0, axisLength);
	return ax;\
}


typedef struct {
	SDL_Window* window;			// Window where everything is rendered
	SDL_Renderer* render;		// Renderer object used to render everything (i.e. like a sort of paintbrush that draws on the screen)
	Uint64 last_frame;			// Timestamp of the last frame that was rendered (used for limiting the number of frames per second to maximum number established in 'constants.h')
	
	InOutHandle ioHandle;		// Struct containing elements useful for IO management in the program
	GeometryHandle geoHandle;	// Struct containing elements useful for geometry management (i.e. points, rotations etc.) in the program
	Axes axesSet;				// Struct containing the set of (3D) axes that are to be drawn in the window

} Appstate;
