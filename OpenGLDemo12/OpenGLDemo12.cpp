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
 *
 * Demo 5:
 * Refactored square drawing, added scaling factor to model/view matrix.
 * Added Pentagon to show combined vertex/color buffer.
 *
 * Demo 6:
 * Switched from glDrawArrays to glDrawElements
 *
 * Demo 7:
 * Added simple animation.
 *
 * Demo 8:
 * Removed square, animation now includes Z component.
 * changed projection matrix from orthoganal to frustum.
 *
 * Demo 9:
 * Changed to 3D solid shapes.
 * Added rotyDegrees to drawTrianglesAt.
 * DrawTrianglesAt now calls glDrawArrays or glDrawElements, as needed.
 * Added depth testing.
 *
 * Demo 10:
 * Switched from SDL to glut.
 * See http://openglbook.com/setting-up-opengl-glew-and-freeglut-in-visual-c/
 *
 * Demo 11:
 * Added glew for VSync, re-added animation
 */
#include <windows.h>
#include <WinGDI.h>

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/GL.h>
#include <glut.h>

#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define CENTER_Z        6.0     // Distance from camera
#define DEPTH_OF_FIELD  5.0

typedef struct {
    const GLushort *indices;
    GLsizei count;
} ShapeInfo;

void setupPyramid(ShapeInfo *pInfo)
{
    typedef struct {
        GLfloat x, y, z;
        GLubyte red, green, blue;
    } VertexInfo;

    static const VertexInfo pyramidData[] = {
        // Bottom
        { 0.0f, 0.f, .5f, 255, 0, 0},
        { 0.433f, 0.f, -.25f, 255, 0, 0},
        { -0.433f, 0.f, -.25f, 255, 0, 0},
        // Side 1
        { -0.433f, 0.f, -.25f, 0, 0, 255},
        { 0.433f, 0.f, -.25f, 0, 0, 255},
        { 0.0f, 0.75f, 0.f, 0, 0, 255},
        // Side 2
        { -0.433f, 0.f, -.25f, 255, 255, 0},
        { 0.0f, 0.f, .5f, 255, 255, 0},
        { 0.0f, 0.75f, 0.f, 255, 255, 0},
        // Side 3
        { 0.0f, 0.f, .5f, 0, 255, 0},
        { 0.433f, 0.f, -.25f, 0, 255, 0},
        { 0.0f, 0.75f, 0.f, 0, 255, 0},
    };

    glVertexPointer(3, GL_FLOAT, sizeof(VertexInfo), &pyramidData[0].x);
    glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(VertexInfo), &pyramidData[0].red);

    pInfo->count = 12;
    pInfo->indices = NULL;
}

void drawTrianglesAt(double x, double y, double z, double rotyDegrees, double scale, ShapeInfo *info)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-x, -y, -z+CENTER_Z, -x, -y, -z, 0., 1., 0.);
    glRotated(rotyDegrees, 0., 1., 0.);
    glScaled(scale, scale, scale);

    if (info->indices) {
        glDrawElements(GL_TRIANGLES, info->count, GL_UNSIGNED_SHORT, info->indices);
    }
    else {
        glDrawArrays(GL_TRIANGLES, 0, info->count);
    }
}

void onDisplay()
{
    static int i = 0;
    if (i < 400) {
        i++;
    }
    double depth = -i/200.;
    double angle = i/30.;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ShapeInfo info;
    setupPyramid(&info);
    drawTrianglesAt(cos(angle), sin(angle), depth, i*3., 2., &info);
    drawTrianglesAt(cos(angle + 2 * M_PI / 3.), sin(angle + 2 * M_PI / 3.), depth, i, 1.5, &info);
    drawTrianglesAt(cos(angle + 4 * M_PI / 3.), sin(angle + 4 * M_PI / 3.), depth, i*10., 1.2, &info);
    glutSwapBuffers();
}

void onKey(unsigned char key, int x, int y)
{
    exit(0);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutCreateWindow(argv[0]);

    // glewInit (called after glutCreateWindow) sets up wglSwapIntervalEXT pointer
    glewInit();
    wglSwapIntervalEXT(1);

    glEnable(GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glMatrixMode(GL_PROJECTION);
    GLdouble ratio = 640.0f / 480.0f;
    // glOrtho(-ratio, ratio, -1., 1., CENTER_Z - DEPTH_OF_FIELD/2, CENTER_Z + DEPTH_OF_FIELD/2);
    glFrustum(-ratio, ratio, -1., 1., CENTER_Z - DEPTH_OF_FIELD/2, CENTER_Z + DEPTH_OF_FIELD/2);

    glutDisplayFunc(onDisplay);
    glutIdleFunc(onDisplay);
    glutKeyboardFunc(onKey);

    glutMainLoop();

    return 0;
}
