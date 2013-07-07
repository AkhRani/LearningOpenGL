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

Demo 10:
* Switched from SDL to glut.
* See http://openglbook.com/setting-up-opengl-glew-and-freeglut-in-visual-c/

Demo 11:
* Added glew for VSync, re-added animation

Demo 12:
* Switched to VBOs

Demo 13:
* Switched from fixed-function pipeline to shader programs.

Demo 14:
* Added model/view matrix to vertex shader.
* Using vmath.h from http://www.opengl-redbook.com/
