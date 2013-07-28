/*
 * Demo 3:
 * Applied a simple translation matrix to draw the square in different
 * locations, and moved vertex data into an array.  Also added depth
 * test to show how the projection matrix changes the Z axis.
 *
 * See README.txt for prerequisites.
 */
#include "SDL.h"
#include "SDL_opengl.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

#undef main     // This un-does SDL's #define main

bool initializeSdl()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return false;
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_Surface *screen = SDL_SetVideoMode(640, 480, 16, SDL_OPENGL);
    if (!screen) {
        printf("Unable to set video mode: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

void drawSquare()
{
    typedef struct {
        GLfloat x, y;
        GLubyte red, green, blue;
    } VertexInfo;

    static const VertexInfo squareVertices[] = {
        {-.5f,  .5f, 255, 0,   0},
        { .5f,  .5f, 0,   255, 0},
        { .5f, -.5f, 0,   0,   255},
        { .5f, -.5f, 0,   0,   255},
        {-.5f, -.5f, 255, 255, 255},
        {-.5f,  .5f, 255, 0,   0}
    };

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 6; i++) {
        glColor3ub(squareVertices[i].red, squareVertices[i].green, squareVertices[i].blue);
        glVertex2f(squareVertices[i].x, squareVertices[i].y);
    }
    glEnd();
}

void drawSquareAt(double x, double y, double z)
{
    // Note: the array declaration does not follow the normal matrix notation.
    // The first column contains the values used to compute the x coordinate.
    GLdouble translationMatrix[] = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        x,   y,   z,   1.0
    };
    glLoadMatrixd(translationMatrix);
    drawSquare();
}

#define USE_PROJECTION_MATRIX 1

int main(int argc, char *argv[])
{
    if (initializeSdl()) {
        // Without the depth test, the most recently drawn primitive
        // is always on top of previously drawn primitives.
        glEnable(GL_DEPTH_TEST);

#if USE_PROJECTION_MATRIX
        glMatrixMode(GL_PROJECTION);
        GLdouble ratio = 640.0f / 480.0f;
        glOrtho(-ratio, ratio, -1, 1, -1, 1);
#endif

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // With no projection matrix, these are window coordinates,
        // which are left-handed.  When using a projection matrix,
        // these are right-handed coordinates, which will be converted
        // to left-handed coordinates by the projection matrix, reversing
        // the direction of the Z axis.
        glMatrixMode(GL_MODELVIEW);
        drawSquareAt(0, 0, 0);
        drawSquareAt(0.5, 0.5, 0.5);

        SDL_GL_SwapBuffers();
        SDL_Delay(3000);
        SDL_Quit();
    }
    return 0;
}
