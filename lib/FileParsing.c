#pragma once
#include <SDL3/SDL.h>
#include <string.h>
#include "../include/FileParsing.h"


Vector3f strToVector3f(const char* str, unsigned long lineNumForDebug) {
    size_t comma1 = 0, comma2 = 0;

    size_t lineLength = strlen(str);
    

    char* tempPtr = strchr(str, ',');
    if (tempPtr == NULL) {
        printf("No first comma (',') found in line %ul\n", lineNumForDebug);
        exit(NULL);
    }
    comma1 = (tempPtr - str) / sizeof(const char);

    tempPtr = strchr(tempPtr+1, ',');
    if (tempPtr == NULL) {
        printf("No second comma (',') found in line %ul\n", lineNumForDebug);
        exit(NULL);
    }
    comma2 = (tempPtr - str) / sizeof(const char);

    Vector3f v = makeVector3f(0, 0, 0);

    char numBuffer[20];

    strncpy(numBuffer, str, comma1);
    numBuffer[comma1] = '\0';
    v.x = atof(numBuffer);

    numBuffer[0] = '\0';
    strncpy(numBuffer, str + comma1 + 1, comma2 - comma1);
    numBuffer[comma2] = '\0';
    v.y = atof(numBuffer);

    numBuffer[0] = '\0';
    size_t endOfFilePos = str + lineLength;
    strncpy(numBuffer, str + comma2 + 1, lineLength - comma2);
    numBuffer[lineLength] = '\0';
    v.z = atof(numBuffer);

    return v;
}


Vector3f* readPointsFromFile(unsigned long* n, const char* fname) {
    FILE* fpointer;
    fopen_s(&fpointer, fname, "r");

    if (fpointer == NULL) {
        perror("Unable to read file\n");
        exit(NULL);
    }
    else {
        // Counting lines in file
        char lineCountBuffer[1024];
        unsigned long lines = 0;
        while (1 != 0) {
            size_t elems = fread(lineCountBuffer, 1, 1024, fpointer);
            if (ferror(fpointer)) {
                perror("Error happened with file\n");
                exit(NULL);
            }

            for (unsigned i = 0; i < elems; i++) {
                if (lineCountBuffer[i] == '\n') {
                    lines++;
                }
            }

            if (feof(fpointer))
                break;
        }

        *n = lines;
        printf("Successfully read %u points from '%s'\n", *n, fname);

        Vector3f* v = (Vector3f*)SDL_calloc(lines, sizeof(Vector3f));

        if (v == NULL) {
            perror("Unable to allocate memory for points array\n");
            exit(NULL);
        }
        else {
            char vectorParseBuffer[50];
            fseek(fpointer, 0, SEEK_SET);   // Moves file cursor back to the start
            for (unsigned long i = 0; i < lines; i++) {
                fgets(vectorParseBuffer, 50, fpointer);
                v[i] = strToVector3f(vectorParseBuffer, i);
            }

            fclose(fpointer);
            return v;
        }
    }
}