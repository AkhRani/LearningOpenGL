/* 
 * Demo 1:
 * Started with the simplest OpenGL program I could make, using SDL.
 *
 * See README.txt for prerequisites.
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
        // Start with a blank canvas
        glClear(GL_COLOR_BUFFER_BIT);

        // Hey, GL!  I'm going to draw triangles!
        // Exercise:  Try GL_POINTS, GL_LINES, GL_QUADS, GL_TRIANGLE_STRIP
        glBegin(GL_TRIANGLES);

        // Each vertex is a corner of a triangle.
        // The first argument is x (horizontal), and the second is y (vertical)
        // The default coordinate space of OpenGL is [-1, 1] in x, y and z.
        glVertex2f(0.0f, 1.0f);
        glVertex2f(1.0f, -1.0f);
        glVertex2f(-1.0f, -1.0f);

        // Exercise:  Try adding more calls to glVertex2f

        // OK, GL.  Done with this kind of shape.
        glEnd();

        // System-defined method to show what we've drawn.
        SDL_GL_SwapBuffers();

        // Wait to give us a chance to see the image
        SDL_Delay(2000);

        // And exit
        SDL_Quit();
    }
    return 0;
}
