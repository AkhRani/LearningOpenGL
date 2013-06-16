/* Adding an orthographic projection to compensate for screen aspect ratio */
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

int main(int argc, char *argv[])
{
    if (initializeSdl()) {
        glMatrixMode(GL_PROJECTION);
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