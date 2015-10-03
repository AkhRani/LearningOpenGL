/*
 * Demo 6:
 * Switched from glDrawArrays to glDrawElements (OpenGL 1.2.1)
 *
 * See README.txt for prerequisites.
 */
#include "SDL.h"
#include "SDL_opengl.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

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
    SDL_Surface *screen = SDL_SetVideoMode(640, 480, 16, SDL_OPENGL);
    if (!screen) {
        printf("Unable to set video mode: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

// Our old data had a lot of redundant information in it.
// Every triangle had all three points in the Vertex and Color
// buffers, even though most of the points were used by more than
// one triangle.  With complex shapes, it would be a lot of wasted
// effort to shuffle that information to the GPU.
// Wouldn't it be nice if we could re-use point information for multiple
// triangles?  Well, as of OpenGL 1.2.1, we can!
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

void drawTrianglesAt(double x, double y, double scale, ShapeInfo *pInfo)
{
    glMatrixMode(GL_MODELVIEW);
    GLdouble modelViewMatrix[] = {
        scale, 0.,    0., 0.,
        0.,    scale, 0., 0.,
        0.,    0.,    1., 0.,
        x,     y,     0., 1.
    };
    glLoadMatrixd(modelViewMatrix);

    // Added in OpenGL 1.2.1
    glDrawElements(GL_TRIANGLES, pInfo->count, GL_UNSIGNED_SHORT, pInfo->indices);
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
        ShapeInfo info;
        setupSquare(&info);
        drawTrianglesAt(0, 0, 0.5, &info);
        drawTrianglesAt(0.5, 0.5, 0.25, &info);

        setupPentagon(&info);
        drawTrianglesAt(-.5, -.5, .5, &info);
        drawTrianglesAt(-.5, .5, .5, &info);
        drawTrianglesAt(.5, -.5, .5, &info);

        SDL_GL_SwapBuffers();
        SDL_Delay(3000);
        SDL_Quit();
    }
    return 0;
}
