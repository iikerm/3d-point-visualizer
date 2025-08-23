#define SDL_MAIN_USE_CALLBACKS 1  /* uses the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdio.h>

//#include "include/constants.h"
#include "include/Appstate.h"
//#include "include/Vector3f.h"
#include "include/FileParsing.h"
#include "include/GeometryMath.h"


// standalone function to draw text so that its contents can be later modified in case I decide to use libraries like SDL_ttf or similar in the future
void drawText(SDL_Renderer* r, float x, float y, const char* str) {
    SDL_RenderDebugText(r, x, y, str);
}


/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {

    Appstate* as = (Appstate*)SDL_calloc(1, sizeof(Appstate));
    if (!as) {
        perror("Unable to create appstate");
        return SDL_APP_FAILURE;
    }

    /* Create the window */
    if (!SDL_CreateWindowAndRenderer("3D Point viewer", WIN_WIDTH, WIN_HEIGHT, 0, &as->window, &as->render)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    as->last_frame = SDL_GetTicks();
    
    
    as->ioHandle = defaultInOutHandle();
    as->geoHandle = defaultGeometryHandle();
    as->axesSet = defaultAxes(100.f);
    
    printf("Reading points from '%s' file", POINTS_FNAME);
    as->geoHandle.pointsArray_3d = readPointsFromFile(&as->geoHandle.nPoints, POINTS_FNAME);
    printf("Points read, drawing window...");

    // Calculating middle point
    as->geoHandle.midPoint = getPointsCenter(as->geoHandle.pointsArray_3d, as->geoHandle.nPoints);

    *appstate = as;
    
    return SDL_APP_CONTINUE;
}


/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    Appstate* as = (Appstate*)appstate;

    if (event->type == SDL_EVENT_QUIT
        || (event->type == SDL_EVENT_KEY_DOWN && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE])) {

        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        as->ioHandle.checkMouse = true;
    }
    if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
        as->ioHandle.checkMouse = false;
    }

    if (event->type == SDL_EVENT_KEY_DOWN && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_TAB]) {
        as->ioHandle.showDebugInfo = !as->ioHandle.showDebugInfo;
    }

    if (event->type == SDL_EVENT_MOUSE_WHEEL) {
        // Remember camzvalue is usually -ve

        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT]) {
            if (as->geoHandle.zCamValue + event->wheel.y / 2 < 0.0) {
                as->geoHandle.zCamValue += event->wheel.y / 2;
            }
        }
        else {
            if (as->geoHandle.zCamValue + event->wheel.y * 5 < 0.0) {
                as->geoHandle.zCamValue += event->wheel.y * 5;
            }
        }
    }

    return SDL_APP_CONTINUE;
}


/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate) {

    Appstate* as = (Appstate*)appstate;

    SDL_FPoint newMousePos;
    SDL_GetMouseState(&newMousePos.x, &newMousePos.y);

    if (as->ioHandle.checkMouse) {
        as->geoHandle.originXY.x += newMousePos.x - as->ioHandle.oldMousePos.x;
        as->geoHandle.originXY.y -= newMousePos.y - as->ioHandle.oldMousePos.y;


    }
    as->ioHandle.oldMousePos = newMousePos;

    const Uint64 now = SDL_GetTicks();

    // z coord is like 'zoom' (i.e. +ve values = more zoom; -ve values = less zoom)
    // Change proportions between x, y & z coords in order to change perspective
    Vector3f cameraPos = makeVector3f(as->geoHandle.zCamValue, as->geoHandle.zCamValue, as->geoHandle.zCamValue);
    
    Vector3f cameraTarget = makeVector3f( 0, 0, 0);          // Point at which the camera is looking
    Vector3f cameraUp = makeVector3f(0, 1, 0);               // Up direction (i.e. +Y axis)


    if ((now - as->last_frame) >= MS_PER_FRAME) {
        as->last_frame = now;


        // ROTATE AROUND X AXIS
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_W]) {
            if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT]) {
                as->geoHandle.rotationAngles.x += ANGLE_STEP_DEG / 2.f;
            }
            else {
                as->geoHandle.rotationAngles.x += ANGLE_STEP_DEG;
            }
        }
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S]) {
            if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT]) {
                as->geoHandle.rotationAngles.x -= ANGLE_STEP_DEG / 2.f;
            }
            else {
                as->geoHandle.rotationAngles.x -= ANGLE_STEP_DEG;
            }
        }

        // ROTATE AROUND Y AXIS
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A]) {
            if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT]) {
                as->geoHandle.rotationAngles.y -= ANGLE_STEP_DEG / 2.f;
            }
            else {
                as->geoHandle.rotationAngles.y -= ANGLE_STEP_DEG;
            }
        }
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_D]) {
            if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT]) {
                as->geoHandle.rotationAngles.y += ANGLE_STEP_DEG / 2.f;
            }
            else {
                as->geoHandle.rotationAngles.y += ANGLE_STEP_DEG;
            }
        }

        float xz_angle = as->geoHandle.rotationAngles.y;
        float yz_angle = as->geoHandle.rotationAngles.x;

        as->geoHandle.rotationAngles = makeVector3f(0, 0, 0);

        // If there is any rotation
        if (yz_angle != 0 || xz_angle != 0) {

            // For every point
            for (unsigned i = 0; i < as->geoHandle.nPoints; i++) {
                rotateVector3f(&as->geoHandle.pointsArray_3d[i], &as->geoHandle.midPoint, yz_angle, xz_angle, 0.0);
            }
        }


        SDL_FPoint* pointsArray = (SDL_FPoint*)SDL_calloc(as->geoHandle.nPoints, sizeof(SDL_FPoint));


        for (unsigned long i = 0; i < as->geoHandle.nPoints; i++) {
            pointsArray[i] = map3dTo2d(
                &as->geoHandle.pointsArray_3d[i], 
                &cameraPos, 
                &cameraTarget, 
                &cameraUp, 
                FOV_Y_DEG, 
                as->geoHandle.originXY.x, as->geoHandle.originXY.y, 
                WIN_WIDTH, WIN_HEIGHT
            );
        }


        SDL_FPoint origin = map3dTo2d(&as->axesSet.origin, &cameraPos, &cameraTarget, &cameraUp, FOV_Y_DEG, as->geoHandle.originXY.x, as->geoHandle.originXY.y, WIN_WIDTH, WIN_HEIGHT);
        SDL_FPoint xaxis = map3dTo2d(&as->axesSet.xAxis, &cameraPos, &cameraTarget, &cameraUp, FOV_Y_DEG, as->geoHandle.originXY.x, as->geoHandle.originXY.y, WIN_WIDTH, WIN_HEIGHT);
        SDL_FPoint yaxis = map3dTo2d(&as->axesSet.yAxis, &cameraPos, &cameraTarget, &cameraUp, FOV_Y_DEG, as->geoHandle.originXY.x, as->geoHandle.originXY.y, WIN_WIDTH, WIN_HEIGHT);
        SDL_FPoint zaxis = map3dTo2d(&as->axesSet.zAxis, &cameraPos, &cameraTarget, &cameraUp, FOV_Y_DEG, as->geoHandle.originXY.x, as->geoHandle.originXY.y, WIN_WIDTH, WIN_HEIGHT);

        SDL_SetRenderDrawColor(as->render, BG_COLOR);
        SDL_RenderClear(as->render);


        const SDL_FPoint xAxisLine[] = { origin, xaxis };
        const SDL_FPoint yAxisLine[] = { origin, yaxis };
        const SDL_FPoint zAxisLine[] = { origin, zaxis };

        // Lines joining points
        SDL_SetRenderDrawColor(as->render, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderLines(as->render, pointsArray, as->geoHandle.nPoints);

        // Sets of axis
        SDL_SetRenderDrawColor(as->render, 0xFF, 0x20, 0x20, 0xFF);
        SDL_RenderLines(as->render, xAxisLine, 2);
        SDL_SetRenderDrawColor(as->render, 0x20, 0xFF, 0x20, 0xFF);
        SDL_RenderLines(as->render, yAxisLine, 2);
        SDL_SetRenderDrawColor(as->render, 0x20, 0x20, 0xFF, 0xFF);
        SDL_RenderLines(as->render, zAxisLine, 2);


        if (as->ioHandle.showDebugInfo) {
            // Drawing points in the canvas
            SDL_SetRenderDrawColor(as->render, 0x77, 0x77, 0x77, 0xFF);
            for (unsigned i = 0; i < as->geoHandle.nPoints; i++) {
                SDL_FRect pt;
                pt.w = pt.h = 4;
                pt.x = pointsArray[i].x - pt.h / 2;
                pt.y = pointsArray[i].y - pt.h / 2;
                SDL_RenderFillRect(as->render, &pt);


                // Draws points' 3D coordinates
                char pointText[50];
                sprintf(pointText, "(%.3f, %.3f, %.3f)\0", as->geoHandle.pointsArray_3d[i].x, as->geoHandle.pointsArray_3d[i].y, as->geoHandle.pointsArray_3d[i].z);
                drawText(as->render, pointsArray[i].x + 2, pointsArray[i].y + 4, pointText);
            }

            SDL_FPoint midPointMapped = map3dTo2d(&as->geoHandle.midPoint, &cameraPos, &cameraTarget, &cameraUp, FOV_Y_DEG, as->geoHandle.originXY.x, as->geoHandle.originXY.y, WIN_WIDTH, WIN_HEIGHT);

            // Draws points' midpoint (i.e. point from which rotations happen) as a ~blue square
            SDL_FRect rm = {
                midPointMapped.x - 2,
                midPointMapped.y - 2,
                4, 4
            };
            SDL_SetRenderDrawColor(as->render, 0x77, 0xBB, 0xBB, 0xFF);
            SDL_RenderFillRect(as->render, &rm);

            char midPointText[50];
            sprintf(midPointText, "(%.3f, %.3f, %.3f)\0", as->geoHandle.midPoint.x, as->geoHandle.midPoint.y, as->geoHandle.midPoint.z);
            drawText(as->render, midPointMapped.x + 2, midPointMapped.y + 4, midPointText);
        }

        SDL_RenderPresent(as->render);
    }

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    Appstate* as = (Appstate*)appstate;
    SDL_free(as->geoHandle.pointsArray_3d);
    SDL_free(appstate);
}