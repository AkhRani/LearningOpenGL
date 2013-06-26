/*
 * Demo 1:
 * Started with the simplest OpenGL program I could make, using SDL.
 *
 * Demo 2:
 * Combined two triangles to make a square, added color, and used an
 * orthographic projection matrix to compensate for the window's
 * aspect ratio.
 *
 * Demo 3:
 * Applied a simple translation matrix to draw the square in different
 * locations, and moved vertex data into an array.
*
 * Demo 4:
 * Switched from glBegin/glEnd to glDrawArrays
 *
 * Demo 5:
 * Refactored square drawing, added scaling factor to model/view matrix.
 * Added Pentagon to show combined vertex/color buffer.
 *
 * Demo 6:
 * Switched from glDrawArrays to glDrawElements
 *
 * Demo 7:
 * Added simple animation.
 *
 * Demo 8:
 * Removed square, animation now includes Z component.
 * changed projection matrix from orthoganal to frustum.
 * Added defines to clarify frustum parameters.
 */
#include "SDL.h"
#include "SDL_opengl.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#undef main     // This un-does SDL's #define main

#define CENTER_Z        2.0     // Distance from camera
#define DEPTH_OF_FIELD  2.0

typedef struct {
    const GLushort *indices;
    GLsizei count;
} ShapeInfo;

bool initializeSdl()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);

    SDL_Surface *screen = SDL_SetVideoMode(640, 480, 16, SDL_OPENGL);
    if (!screen) {
        printf("Unable to set video mode: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

void setupPentagon(ShapeInfo *pInfo)
{
    typedef struct {
        GLfloat x, y;
        GLubyte red, green, blue;
    } VertexInfo;

    static const VertexInfo pentagonData[] = {
        { 0.f, .5f,    255, 0, 0 },
        { .47f, .15f,  0, 255, 0 },
        { .29f, -.4f,  0, 0, 255 },
        { -.29f, -.4f, 255, 255, 255 },
        { -.47f, .15f, 255, 255, 0 },
    };

    glVertexPointer(2, GL_FLOAT, sizeof(VertexInfo), &pentagonData[0].x);
    glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(VertexInfo), &pentagonData[0].red);

    static const GLushort indices[] = { 0, 1, 2, 2, 3, 0, 0, 3, 4 };

    pInfo->count = 9;
    pInfo->indices = indices;
}

void drawTrianglesAt(double x, double y, double z, double scale, ShapeInfo *pInfo)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-x, -y, -z+CENTER_Z, -x, -y, -z, 0., 1., 0.);
    glScaled(scale, scale, 1.);

    glDrawElements(GL_TRIANGLES, pInfo->count, GL_UNSIGNED_SHORT, pInfo->indices);
}

int main(int argc, char *argv[])
{
    if (initializeSdl()) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glMatrixMode(GL_PROJECTION);
        GLdouble ratio = 640.0f / 480.0f;
        // glOrtho(-ratio, ratio, -1., 1., CENTER_Z - DEPTH_OF_FIELD/2, CENTER_Z + DEPTH_OF_FIELD/2);
        glFrustum(-ratio, ratio, -1., 1., CENTER_Z - DEPTH_OF_FIELD/2, CENTER_Z + DEPTH_OF_FIELD/2);

        for (int i = 0; i < 400; i++) {
            double depth = 1. - i/200.;     // from +1 to -1
            double sinVal = sin(i/30.);
            double cosVal = cos(i/30.);
            glClear(GL_COLOR_BUFFER_BIT);
            ShapeInfo info;
            setupPentagon(&info);
            drawTrianglesAt(-.707 * cosVal, -.707 * sinVal, depth, .5, &info);
            drawTrianglesAt(-.707 * cosVal, .707 * sinVal, depth, .4, &info);
            drawTrianglesAt(.707 * cosVal, -.707 * sinVal, depth, .3, &info);

            SDL_GL_SwapBuffers();
        }
        SDL_Quit();
    }
    return 0;
}
