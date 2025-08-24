#pragma once
#include <math.h>

/*
Struct designed to hold 3 different floating point values to represent a point
*/
typedef struct {
	float x;
	float y;
	float z;
} Vector3f;

/*
Returns an instance of Vector3f containing the values received
*/
inline Vector3f makeVector3f(float x, float y, float z) {
    Vector3f v;
    v.x = x;
    v.y = y;
    v.z = z;

    return v;
}

/*
Performs the operation a - b, by subtracting each component of b from their corresponding component in a
*/
inline Vector3f subtract(const Vector3f* a, const Vector3f* b) {
    return makeVector3f(
        a->x - b->x,
        a->y - b->y,
        a->z - b->z
    );
}

/*
Performs the operation a + b, by adding each component of b to their corresponding component in a
*/
inline Vector3f add(const Vector3f* a, const Vector3f* b) {
    return makeVector3f(
        a->x + b->x,
        a->y + b->y,
        a->z + b->z
    );
}

/*
Calculates the dot (or scalar) product between vector a and vector b.
It can be used to calculate the length of a vector by obtaining the square root of the dot product of said vector with itself
*/
inline double dotProduct(const Vector3f* a, const Vector3f* b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

/*
Calculates the cross (or vector) product between vector a and vector b.
Its result is a vector that is perpendicular to both other vectors.
*/
inline Vector3f crossProduct(const Vector3f* a, const Vector3f* b) {
    return makeVector3f(
        a->y * b->z - a->z * b->y,
        a->z * b->x - a->x * b->z,
        a->x * b->y - a->y * b->x
    );
}

/*
Calculates the unitary vector of v.
This is a vector that points in the same direction as v, but has a length of 1.
*/
inline Vector3f createUnitaryVector(const Vector3f* v) {
    
    if ((v->x == 0) && (v->y == 0) && (v->z == 0)) {
        /* This is to avoid divisions by zero(i.e. if all the components of v are zero,
        the length of the vector is also 0, so there will be divisions by 0 later on)*/
        return makeVector3f(0, 0, 0);
    }

    float length = (float)sqrt(dotProduct(v, v));          // Calculates vector length

    return makeVector3f(
        v->x / length,
        v->y / length,
        v->z / length
    );
}