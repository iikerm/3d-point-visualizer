#pragma once
#include "../include/GeometryMath.h"
#include <stdio.h>

Vector3f getPointsCenter(Vector3f points[], unsigned count) {
    if (count == 0) {
        printf("Invalid point count to get center: %u\n", count);
        exit(NULL);
    }

    Vector3f mid = points[0];
    for (unsigned i = 1; i < count; i++) {
        mid.x = (mid.x + points[i].x) / 2.f;
        mid.y = (mid.y + points[i].y) / 2.f;
        mid.z = (mid.z + points[i].z) / 2.f;
    }

    return mid;
}



SDL_FPoint map3dTo2d(
    const Vector3f* pt,                 // Point to map
    const Vector3f* cameraPos,          // Position of the camera in 3D space
    const Vector3f* cameraTarget,       // Target point that the camera is looking at
    const Vector3f* cameraUpDirection,  // Vector that indicates the 'up' direction
    double y_fov_deg,
    float originX, float originY,
    int screenWidth, int screenHeight)
{
    /*
    We calculate the vectors that tell us the direction in which the camera is pointing(i.e.positive axis' for all 3 sets of coordinates):

        - Forward vector: Indicates the direction of the vector facing forwards from the camera
        - Right vector:   Indicates the positive direction in the sidewise axis (i.e. the rightwards axis) from the camera
        - Up vector       Indicates the positive vertical direction from the camera.
    */
    const Vector3f tempForward = subtract(cameraTarget, cameraPos);             // Intermediate step for calculating 'forward' vector
    const Vector3f forward = createUnitaryVector(&tempForward);                 // unitary vector pointing towards cameraTarget
    
    const Vector3f tempRight = crossProduct(cameraUpDirection, &forward);       // Intermediate step for calculating 'right' vector
    const Vector3f right = createUnitaryVector(&tempRight);     // Cross product of forward direction and up direction (i.e. perpendicular vector to both of these)

    const Vector3f up = crossProduct(&forward, &right);         // Same principle as with right vector. Doesn't need to be made unitary because both forward and right already are unitary vectors


    Vector3f relativePoint = subtract(pt, cameraPos);    // Point relative to the camera's position

    // Point's coordinates in 3D, as 'seen' by the camera
    Vector3f coordsFromCamera = makeVector3f(
        (float)dotProduct(&right, &relativePoint),
        (float)dotProduct(&up, &relativePoint),
        (float)dotProduct(&forward, &relativePoint)
    );
    /*
    From the lines above, we get that:
        X axis is the horizontal axis
        Y axis is the vertical axis
        Z axis represents distance away from the camera
    */

    double y_fov_rad = y_fov_deg * TO_RAD_CONSTANT;
    double f_y = screenHeight / tan(y_fov_rad);
    double f_x = f_y * ((double)screenWidth / (double)screenHeight);

    SDL_FPoint out;
    out.x = (float)(coordsFromCamera.x * f_x / coordsFromCamera.z + originX);
    out.y = (float)(-(originY + coordsFromCamera.y * f_y / coordsFromCamera.z));

    return out;
}


bool rotateVector3f(Vector3f* p, const Vector3f* origin, double x_deg, double y_deg, double z_deg) {
    // https://en.wikipedia.org/wiki/Rotation_matrix#In_three_dimensions

    if (x_deg == 0.0 && y_deg == 0.0 && z_deg == 0.0) {
        return false;
    }

    *p = subtract(p, origin);

    if (x_deg != 0.0) {
        double x_rad = x_deg * TO_RAD_CONSTANT;
        double xsin = sin(x_rad);
        double xcos = cos(x_rad);

        /*
        Rotation matrix multiplication for rotation around X-Axis:
        | 1      0       0    | * | p.x |
        | 0      cos     -sin |   | p.y |
        | 0      sin     cos  |   | p.z |
        */

        *p = makeVector3f(
            (float)(1 * p->x + 0 * p->y + 0 * p->z),
            (float)(0 * p->x + xcos * p->y - xsin * p->z),
            (float)(0 * p->x + xsin * p->y + xcos * p->z)
        );
    }

    if (y_deg != 0.0) {
        double y_rad = y_deg * TO_RAD_CONSTANT;
        double ysin = sin(y_rad);
        double ycos = cos(y_rad);

        /*
        Rotation matrix multiplication for rotation around Y-Axis:
        | cos    0       sin  | * | p.x |
        | 0      1       0    |   | p.y |
        | -sin   0       cos  |   | p.z |
        */

        *p = makeVector3f(
            (float)(ycos * p->x + 0 * p->y + ysin * p->z),
            (float)(0 * p->x + 1 * p->y + 0 * p->z),
            (float)(-ysin * p->x + 0 * p->y + ycos * p->z)
        );
    }

    if (z_deg != 0.0) {
        double z_rad = z_deg * TO_RAD_CONSTANT;
        double zsin = sin(z_rad);
        double zcos = cos(z_rad);

        /*
        Rotation matrix multiplication for rotation around Y-Axis:
        | cos    -sin    0    | * | p.x |
        | sin    cos     0    |   | p.y |
        | 0      0       1    |   | p.z |
        */

        *p = makeVector3f(
            (float)(zcos * p->x - zsin * p->y + 0 * p->z),
            (float)(zsin * p->x + zcos * p->y + 0 * p->z),
            (float)(0 * p->x + 0 * p->y + 1 * p->z)
        );
    }

    *p = add(p, origin);

    return true;
}