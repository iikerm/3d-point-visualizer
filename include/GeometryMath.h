#pragma once

#include <SDL3/SDL.h>
#include "Vector3f.h"
#include "constants.h"

Vector3f getPointsCenter(Vector3f points[], unsigned count);

SDL_FPoint map3dTo2d(
    const Vector3f* pt,                 // Point to map
    const Vector3f* cameraPos,          // Position of the camera in 3D space
    const Vector3f* cameraTarget,       // Target point that the camera is looking at
    const Vector3f* cameraUpDirection,  // Vector that indicates the 'up' direction
    double y_fov_deg,
    float originX, float originY,
    int screenWidth, int screenHeight);

bool rotateVector3f(Vector3f* p, const Vector3f* origin, double x_deg, double y_deg, double z_deg);