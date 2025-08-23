#pragma once

#include <stdio.h>
#include "Vector3f.h"
#include "constants.h"

/*
Function that receives a string (char[]) containing 3 numbers (float, int, double etc.) separated by commas
and returns a Vector3f element containing these numbers in the x, y, z fields in the order they appeared in the string
*/
Vector3f strToVector3f(const char* str, unsigned long lineNumForDebug);

/*
Function that reads the file specified by 'fname' and returns a pointer to an array of Vector3f's that
represent the points read from said file
*/
Vector3f* readPointsFromFile(unsigned long* n, const char* fname);