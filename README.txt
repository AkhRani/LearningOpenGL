Prerequisites for Demo 1 - Demo 9:
 *   Download SDL-devel-1.2.15-VC.zip from http://www.libsdl.org/download-1.2.php
 *   If View->Property Manager is not available, enable it using Tools -> Settings -> Expert settings.
 *   Use the Property Manager (View->Property Manager) to edit the sdl_project property page.
       Under User Macros, set SDL_DIR to the un-zipped SDL directory, e.g. C:\SDL-devel-1.2.15-VC\SCL-1.2.15

Demo 1:
* Started with the simplest OpenGL program I could make, using SDL.

Demo 2:
* Combined two triangles to make a square, added color, and used an
orthographic projection matrix to compensate for the window's
aspect ratio.

Demo 3:
* Applied a simple translation matrix to draw the square in different
locations, and moved vertex data into an array.

Demo 4:
* Switched from glBegin/glEnd to glDrawArrays

Demo 5:
* Refactored square drawing, added scaling factor to model/view matrix.
* Added Pentagon to show combined vertex/color buffer.

Demo 6:
* Switched from glDrawArrays to glDrawElements

Demo 7:
* Added simple animation.

Demo 8:
* Removed square, animation now includes Z component.
* Using gluLookAt for model/view matrix.
* changed projection matrix from orthoganal to frustum.
* Added defines to clarify frustum parameters.

Demo 9:
* Changed to 3D solid shapes.
* Added rotyDegrees to drawTrianglesAt.
* DrawTrianglesAt now calls glDrawArrays or glDrawElements, as needed.
* Added depth testing.


Prerequisites for Demos 10-14:
 * Download freeglut from http://files.transmissionzero.co.uk/software/development/GLUT/freeglut-MSVC.zip
 * Download glew from https://sourceforge.net/projects/glew/files/glew/1.9.0/glew-1.9.0-win32.zip/download
 * Unzip freeglut and glew.
 * Edit the glut_project property pages, and go to User Macros.
    + Set FREEGLUT_DIR to the freeglut directory, e.g. C:\freeglut-2.8.1 
    + Set GLEW_DIR to the glew directory, e.g. C:\glew-1.9.0-win32\glew-1.9.0


Demo 10:
* Switched from SDL to glut.
* See http://openglbook.com/setting-up-opengl-glew-and-freeglut-in-visual-c/

Demo 11:
* Added glew for VSync, re-added animation

Demo 12:
* Switched to VBOs

Demo 13:
* Switched from fixed-function pipeline to shader programs.


Additional prerequisites for Demo 14+:
 * Download http://www.opengl-redbook.com/Code/oglpg-8th-edition.zip
 * Make sure oglpg-8th-edition/include is in your include path.
 * Edit the glut_project property page.
     Under User Macros, set OGLPG_DIR to the un-zipped directory,
     e.g. C:\oglpg-8th-edition


Demo 14:
* Added model/view matrix to vertex shader.
* Using vmath.h from http://www.opengl-redbook.com/
