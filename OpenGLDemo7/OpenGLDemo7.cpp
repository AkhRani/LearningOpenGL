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
 * locations.
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
 */
#include "SDL.h"
#include "SDL_opengl.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#undef main     // This un-does SDL's #define main

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

void setupSquare(ShapeInfo *pInfo)
{
    static const GLfloat squareCoords[] = {
        -0.5f, 0.5f,
        0.5f, 0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f,
    };
    glVertexPointer(2, GL_FLOAT, 0, squareCoords);

    static const GLfloat squareColors[] = {
        1.0f, 0, 0,
        0, 1.0f, 0,
        0, 0, 1.0f,
        1.0f, 1.0f, 1.0f,
    };
    glColorPointer(3, GL_FLOAT, 0, squareColors);

    static const GLushort indices[] = { 0, 1, 2, 2, 3, 0 };

    pInfo->count = 6;
    pInfo->indices = indices;
}

void setupPentagon(ShapeInfo *pInfo)
{
    typedef struct {
        GLfloat x, y;
        GLubyte red, green, blue;
    } CoordInfo;

    static const CoordInfo pentagonData[] = {
        { 0.f, .5f,    255, 0, 0 },
        { .47f, .15f,  0, 255, 0 },
        { .29f, -.4f,  0, 0, 255 },
        { -.29f, -.4f, 255, 255, 255 },
        { -.47f, .15f, 255, 255, 0 },
    };

    glVertexPointer(2, GL_FLOAT, sizeof(CoordInfo), &pentagonData[0].x);
    glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(CoordInfo), &pentagonData[0].red);

    static const GLushort indices[] = { 0, 1, 2, 2, 3, 0, 0, 3, 4 };

    pInfo->count = 9;
    pInfo->indices = indices;
}

void drawTrianglesAt(double x, double y, double scale, ShapeInfo *info)
{
    glMatrixMode(GL_MODELVIEW);
    GLdouble modelViewMatrix[] = {
        scale, 0.,    0., 0.,
        0.,    scale, 0., 0.,
        0.,    0.,    1., 0.,
        x,     y,     0., 1.
    };
    glLoadMatrixd(modelViewMatrix);

    glDrawElements(GL_TRIANGLES, info->count, GL_UNSIGNED_SHORT, info->indices);
}

int main(int argc, char *argv[])
{
    if (initializeSdl()) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glMatrixMode(GL_PROJECTION);
        GLdouble ratio = 640.0f / 480.0f;
        glOrtho(-ratio, ratio, -1, 1, -1, 1);

        for (int i = 0; i < 400; i++) {
            double sinVal = sin(i/30.);
            double cosVal = cos(i/30.);
            glClear(GL_COLOR_BUFFER_BIT);
            ShapeInfo info;
            setupSquare(&info);
            drawTrianglesAt(0, 0, 0.5, &info);
            drawTrianglesAt(0.5 * cosVal, 0.5 * sinVal, 0.25, &info);

            setupPentagon(&info);
            drawTrianglesAt(-.5 * cosVal, -.5 * sinVal, .5, &info);
            drawTrianglesAt(-.5 * cosVal, .5 * sinVal, .5, &info);
            drawTrianglesAt(.5 * cosVal, -.5 * sinVal, .5, &info);

            SDL_GL_SwapBuffers();
        }
        SDL_Quit();
    }
    return 0;
}