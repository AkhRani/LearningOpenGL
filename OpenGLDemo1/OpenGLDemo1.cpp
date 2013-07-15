/* 
 * Demo 1:
 * Started with the simplest OpenGL program I could make, using SDL.
 *
 * Prerequisites:
 *   Download SDL-devel-1.2.15-VC.zip from http://www.libsdl.org/download-1.2.php
 *   Set the environment variable SDL_DIR to the un-zipped SDL directory, e.g.
 *      SDL_DIR=C:\SDL-devel-1.2.15-VC\SCL-1.2.15
 */
#include "SDL.h"
#include "SDL_opengl.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

#undef main     // This un-does SDL's #define main

bool InitializeSdl()
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
    

int main(int argc, char *argv[])
{
    if (InitializeSdl()) {
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_TRIANGLES);
        glVertex2f(0.0f, 1.0f);
        glVertex2f(1.0f, -1.0f);
        glVertex2f(-1.0f, -1.0f);
        glEnd();

        SDL_GL_SwapBuffers();
        // Wait to give us a chance to see the image
        SDL_Delay(2000);
        SDL_Quit();
    }
    return 0;
}
