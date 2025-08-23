#pragma once

#include <SDL3/SDL.h>
#include <stdbool.h>
#include "Vector3f.h"
#include "constants.h"


typedef struct {
	bool checkMouse;
	bool showDebugInfo;
	SDL_FPoint oldMousePos;
} InOutHandle;

/*
Returns an InOutHandle element initialized with the default values
*/
inline InOutHandle defaultInOutHandle() {
	InOutHandle ioHandle;

	ioHandle.checkMouse = false;
	ioHandle.showDebugInfo = false;
	SDL_GetMouseState(&ioHandle.oldMousePos.x, &ioHandle.oldMousePos.y);

	return ioHandle;
}


typedef struct {
	Vector3f rotationAngles;
	float zCamValue;
	unsigned long nPoints;
	Vector3f midPoint;
	SDL_FPoint originXY;

	Vector3f* pointsArray_3d;		// MUST BE INITIALIZED WITH AN ARRAY OF POINTS BEFORE USE
} GeometryHandle;

/*
Returns a GeometryHandle element initialized with the default values. POINTS ARRAY IS NOT INITIALIZED.
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
	Vector3f origin;
	Vector3f xAxis;
	Vector3f yAxis;
	Vector3f zAxis;
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
	SDL_Window* window;
	SDL_Renderer* render;
	Uint64 last_frame;
	
	InOutHandle ioHandle;
	GeometryHandle geoHandle;
	Axes axesSet;

} Appstate;
