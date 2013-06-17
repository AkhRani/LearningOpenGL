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
    glBegin(GL_TRIANGLES);
    glColor3ub(255, 0, 0);
    glVertex2f(-0.5f, 0.5f);
    glColor3ub(0, 255, 0);
    glVertex2f(0.5f, 0.5f);
    glColor3ub(0, 0, 255);
    glVertex2f(0.5f, -0.5f);

    glColor3ub(0, 0, 255);
    glVertex2f(0.5f, -0.5f);
    glColor3ub(255, 255, 255);
    glVertex2f(-0.5f, -0.5f);
    glColor3ub(255, 0, 0);
    glVertex2f(-0.5f, 0.5f);
    glEnd();
}

void drawSquareAt(double x, double y)
{
    GLdouble translationMatrix[] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        x,    y, 0.0f, 1.0f
    };
    glLoadMatrixd(translationMatrix);
    drawSquare();
}

int main(int argc, char *argv[])
{
    if (initializeSdl()) {
        glMatrixMode(GL_PROJECTION);
        GLdouble ratio = 640.0f / 480.0f;
        glOrtho(-ratio, ratio, -1, 1, -1, 1);
        glMatrixMode(GL_MODELVIEW);

        glClear(GL_COLOR_BUFFER_BIT);
        drawSquareAt(0, 0);
        drawSquareAt(0.5, 0.5);

        SDL_GL_SwapBuffers();
        SDL_Delay(5000);
        SDL_Quit();
    }
    return 0;
}
