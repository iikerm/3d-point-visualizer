#pragma once

#include <SDL3/SDL.h>
#include "Vector3f.h"
#include "constants.h"

/*
Function that calculates the middle or 'average' point from a given array of points in 3D
*/
Vector3f getPointsCenter(Vector3f points[], unsigned count);

/*
Function that receives a point in 3D and returns its 2D equivalent for this specific set of parameters
*/
SDL_FPoint map3dTo2d(
    const Vector3f* pt,                 // Point to map
    const Vector3f* cameraPos,          // Position of the camera in 3D space
    const Vector3f* cameraTarget,       // Target point that the camera is looking at
    const Vector3f* cameraUpDirection,  // Vector that indicates the 'up' direction
    double y_fov_deg,                   // Camera field of view in degrees
    float originX, float originY,       // 2D origin coordinates in the screen
    int screenWidth, int screenHeight   // Screen dimensions in pixels
);

/*
Function that rotates a point in 3D around the given origin, and along each axis the given number of degrees (i.e. x_deg is the amount of degrees rotated around the x axis)
*/
bool rotateVector3f(Vector3f* p, const Vector3f* origin, double x_deg, double y_deg, double z_deg);