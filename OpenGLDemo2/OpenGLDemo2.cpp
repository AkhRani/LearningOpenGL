/* 
 * Demo 2:
 * Combined two triangles to make a square, added color, and used an
 * orthographic projection matrix to compensate for the window's
 * aspect ratio.
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
    glBegin(GL_TRIANGLES);
    // For each vertex, we now assign a color before we set the position
    // Colors can be unsigned bytes, in which
    // case the range for each component is 0-255.
    glColor3ub(255, 0, 0);
    glVertex2f(-0.5f, 0.5f);

    glColor3ub(0, 255, 0);
    glVertex2f(0.5f, 0.5f);

    glColor3ub(0, 0, 255);
    glVertex2f(0.5f, -0.5f);

    // Colors can also be floats, in which case the range is 0 - 1.0
    glColor3f(0, 0, 1.0);
    glVertex2f(0.5f, -0.5f);

    glColor3f(1.0, 1.0, 1.0);
    glVertex2f(-0.5f, -0.5f);

    // Exercise:  What if this color doesn't match the color of the first vertex?
    glColor3f(1.0, 0, 0);
    glVertex2f(-0.5f, 0.5f);
    glEnd();
}

int main(int argc, char *argv[])
{
    if (initializeSdl()) {
        // Tell GL what matrix we want to modify
        glMatrixMode(GL_PROJECTION);

        // Orthographic projection, which ignores depth.
        GLdouble ratio = 640.0f / 480.0f;
        glOrtho(-ratio, ratio, -1, 1, -1, 1);

        glClear(GL_COLOR_BUFFER_BIT);
        drawSquare();

        SDL_GL_SwapBuffers();
        SDL_Delay(2000);
        SDL_Quit();
    }
    return 0;
}
