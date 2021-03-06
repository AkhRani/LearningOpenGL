/*
 * Demo 5:
 * Refactored square drawing, added scaling factor to model/view matrix.
 * Added Pentagon to show combined vertex/color buffer.
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

int setupSquare()
{
    // This is basically the same code as last time, using
    // glVertexPointer and glColorPointer with contiguous data.
    static const GLfloat squareCoords[] = {
        -0.5f, 0.5f,   0.5f, 0.5f,   0.5f, -0.5f,
        0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f
    };
    glVertexPointer(2, GL_FLOAT, 0, squareCoords);

    static const GLfloat squareColors[] = {
        1.0f, 0, 0,
        0, 1.0f, 0,
        0, 0, 1.0f,
        0, 0, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 0, 0
    };
    glColorPointer(3, GL_FLOAT, 0, squareColors);

    return 6;
}

int setupPentagon()
{
    // If we have our nicely organized shape information
    typedef struct {
        GLfloat x, y;
        GLubyte red, green, blue;
    } VertexInfo;

    static const VertexInfo pentagonData[] = {
        { 0.f, .5f, 255, 0, 0 },
        { .47f, .15f, 0, 255, 0 },
        { .29f, -.4f, 0, 0, 255 },
        { .29f, -.4f, 0, 0, 255 },
        { -.29f, -.4f, 255, 255, 255 },
        { 0.f, .5f, 255, 0, 0 },
        { 0.f, .5f, 255, 0, 0 },
        { -.29f, -.4f, 255, 255, 255 },
        { -.47f, .15f, 255, 255, 0 },
    };

    // We can simply tell OpenGL where the relevant info starts, and how far
    // it is from one tuple of values to the next.
    glVertexPointer(2, GL_FLOAT, sizeof(VertexInfo), &pentagonData[0].x);
    glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(VertexInfo), &pentagonData[0].red);

    return 9;
}

void drawTrianglesAt(double x, double y, double scale, int indexCount)
{
    glMatrixMode(GL_MODELVIEW);
    GLdouble modelViewMatrix[] = {
        scale, 0.,    0., 0.,
        0.,    scale, 0., 0.,
        0.,    0.,    1., 0.,
        x,     y,     0., 1.
    };
    glLoadMatrixd(modelViewMatrix);

    glDrawArrays(GL_TRIANGLES, 0, indexCount);
}

int main(int argc, char *argv[])
{
    if (initializeSdl()) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glMatrixMode(GL_PROJECTION);
        GLdouble ratio = 640.0f / 480.0f;
        glOrtho(-ratio, ratio, -1, 1, -1, 1);

        glClear(GL_COLOR_BUFFER_BIT);
        int indexCount = setupSquare();
        drawTrianglesAt(0, 0, 0.5, indexCount);
        drawTrianglesAt(0.5, 0.5, 0.25, indexCount);

        indexCount = setupPentagon();
        drawTrianglesAt(-.5, -.5, .5, indexCount);
        drawTrianglesAt(-.5, .5, .5, indexCount);
        drawTrianglesAt(.5, -.5, .5, indexCount);

        SDL_GL_SwapBuffers();
        SDL_Delay(3000);
        SDL_Quit();
    }
    return 0;
}
