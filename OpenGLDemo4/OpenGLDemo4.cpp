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
    static const GLfloat squareCoords[] = {
        -0.5f, 0.5f,   0.5f, 0.5f,   0.5f, -0.5f,
        0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f
    };
    glVertexPointer(2, GL_FLOAT, 0, squareCoords);

    static const GLubyte squareColors[] = {
        255, 0, 0,
        0, 255, 0,
        0, 0, 255,
        0, 0, 255,
        255, 255, 255,
        255, 0, 0
    };
    glColorPointer(3, GL_UNSIGNED_BYTE, 0, squareColors);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawSquareAt(double x, double y)
{
    GLdouble translationMatrix[] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        x,    y,    0.0f, 1.0f
    };
    glLoadMatrixd(translationMatrix);
    drawSquare();
}

int main(int argc, char *argv[])
{
    if (initializeSdl()) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glMatrixMode(GL_PROJECTION);
        GLdouble ratio = 640.0f / 480.0f;
        glOrtho(-ratio, ratio, -1, 1, -1, 1);
        glMatrixMode(GL_MODELVIEW);

        glClear(GL_COLOR_BUFFER_BIT);
        drawSquareAt(0, 0);
        drawSquareAt(0.5, 0.5);

        SDL_GL_SwapBuffers();
        SDL_Delay(3000);
        SDL_Quit();
    }
    return 0;
}
