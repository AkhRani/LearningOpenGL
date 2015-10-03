/*
 * Demo 12:
 * Switched to VBOs (OpenGL 2.0)
 *
 * See README.txt for prerequisites.
 */
#include <windows.h>
#include <WinGDI.h>

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/GL.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stddef.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define CENTER_Z        6.0     // Distance from camera
#define DEPTH_OF_FIELD  5.0

typedef struct {
  GLfloat x, y, z;
  GLubyte red, green, blue;
} VertexInfo;

typedef struct {
    GLsizei count;
    GLuint vboId;
} ShapeInfo;

// Get ready to jump all the way to OpenGL 2.0!  In the older versions of
// OpenGL, vertex data was stored on the client.  However, as the spec puts it,
// "It is sometimes desirable to store frequently used client
// data, such as vertex array data, in high-performance server memory.
// GL buffer objects provide a mechanism that clients can use to allocate,
// initialize, and render from such memory."

void setupPyramid(ShapeInfo *pInfo)
{
    GLuint vboId(0);

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

    // This is our first "Gen" call.  OpenGL has a lot of variations on this.
    // This is how you create "object names" (OpenGL insists on referring to
    // things as "names" when all they are is a non-zero integer).  The first
    // argument is the number of objects to create, and the second is a buffer
    // to hold the names.  When a name is initially created, it has a type and
    // not much else.
    glGenBuffers(1, &vboId);

    // This function tells OpenGL, "From now on, when I say GL_ARRAY_BUFFER,
    // I'm talking about this buffer object."  
    // Note:  If we wanted to put array indices (remember Demo 6) into a buffer
    // object, we would use GL_ELEMENT_ARRAY_BUFFER.
    glBindBuffer(GL_ARRAY_BUFFER, vboId);

    // And this function provides data for the array buffer (which we just
    // bound to our BO.).  GL_STATIC_DRAW is a hint that describes how this
    // data is used.  See the spec for other options and their meanings.
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidData), pyramidData, GL_STATIC_DRAW);

    pInfo->count = 12;
    pInfo->vboId = vboId;
}

// Second shape, to demonstrate switching between VBOs
void setupFins(ShapeInfo *pInfo)
{
  GLuint vboId(0);

  static const VertexInfo finData[] = {
    // Triangle 1
    { -.5f, -.5f, 0.f, 255, 0, 0 },
    { .5f, -.5f, 0.f, 255, 0, 0 },
    { 0.f, .5f, 0.f, 255, 0, 0 },
    // Triangle 2
    { 0.f, -.5f, -.5f, 0, 255, 0 },
    { 0.f, -.5f, .5f, 0, 255, 0 },
    { 0.f, .5f, 0.f, 0, 255, 0 },
  };

  // This time, without the excessive comments
  glGenBuffers(1, &vboId);
  glBindBuffer(GL_ARRAY_BUFFER, vboId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(finData), finData, GL_STATIC_DRAW);

  pInfo->count = 6;
  pInfo->vboId = vboId;
}

void drawTrianglesAt(double x, double y, double z, double rotyDegrees, double scale, ShapeInfo *pInfo)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(x, y, z - CENTER_Z);
    glRotated(rotyDegrees, 0., 1., 0.);
    glScaled(scale, scale, scale);

    // We have multiple VBOs, so we have to tell OpenGL which one to use
    // Exercise:  What happens if you don't call this?
    glBindBuffer(GL_ARRAY_BUFFER, pInfo->vboId);

    // Note that the data has already been put into the buffer object, so the
    // "Pointer" commands now only provide an offset instead of an actual
    // pointer.  (See glspec20, section 2.9.1.)
    glVertexPointer(3, GL_FLOAT, sizeof(VertexInfo), (GLvoid*)offsetof(VertexInfo, x));
    glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(VertexInfo), (GLvoid*)offsetof(VertexInfo, red));

    glDrawArrays(GL_TRIANGLES, 0, pInfo->count);
}

ShapeInfo g_Pyramid;
ShapeInfo g_Fins;

void onDisplay()
{
    static int i = 0;
    if (i < 400) {
        i++;
    }
    double z = -i/200.;
    double angle = i/30.;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawTrianglesAt(cos(angle), sin(angle), z, i*3., 2., &g_Pyramid);
    drawTrianglesAt(cos(angle + 2 * M_PI / 3.), sin(angle + 2 * M_PI / 3.), z, i, 1.5, &g_Fins);
    drawTrianglesAt(cos(angle + 4 * M_PI / 3.), sin(angle + 4 * M_PI / 3.), z, i*10., 1.2, &g_Pyramid);
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
    glFrustum(-ratio, ratio, -1., 1., CENTER_Z - DEPTH_OF_FIELD/2, CENTER_Z + DEPTH_OF_FIELD/2);

    setupPyramid(&g_Pyramid);
    setupFins(&g_Fins);
    glutDisplayFunc(onDisplay);
    glutIdleFunc(onDisplay);
    glutKeyboardFunc(onKey);
    glutMainLoop();

    return 0;
}
