/*
 * Demo 12.5:
 * Added minimal shader programs.
 *
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
    GLsizei count;
    GLuint vboId;
} ShapeInfo;

// See GLSLangSpec.Full.120.8.pdf for more information about built-in attributes and varyings.
// These are deprecated in later versions of the GLSL.
void setupShaders()
{
  GLchar infoLog[4096];

  // gl_Vertex and gl_Color are Built-in attributes, set by glVertex, glVertexPointer, etc.
  const GLchar *vertShaderSource[] = {
    "#version 120\n"
    "void main() {\n"
    "    gl_Position = gl_Vertex;"    // Every version 1.2 vertex shader must write to gl_Position
    "    gl_FrontColor = gl_Color;"   // gl_FrontColor is a built-in varying, see below.
    "}\n"
  };
  GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertShader, 1, vertShaderSource, NULL);
  glCompileShader(vertShader);
  glGetShaderInfoLog(vertShader, 4096, NULL, infoLog);

  // Here, gl_Color is not the same as gl_Color in the vertex shader.
  // gl_Color is a built-in "varying" variable, which will be an interpolation
  // between the colors set by the vertex shader for the corners of the
  // geometry.  Depending on the orientation of the geometry, either the
  // gl_FrontColor or gl_BackColor will be used.
  const GLchar *fragShaderSource[] = {
    "#version 120\n"
    "void \n"
    "main() {\n"
    "    gl_FragColor = gl_Color;\n"  // Fragment shaders write to gl_FragColor (among other things)
    "}\n"
  };
  GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragShader, 1, fragShaderSource, NULL);
  glCompileShader(fragShader);
  glGetShaderInfoLog(fragShader, 4096, NULL, infoLog);


  GLuint program = glCreateProgram();

  // We can attach a fragment shader without attaching a vertex shader
  // Note that the vertex shader we have simply passes through the vertex
  // position without using the transforming matrices, so all the lovely
  // animation will go away if you un-comment the following line.
  // glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);
  glLinkProgram(program);
  glGetProgramInfoLog(program, 4096, NULL, infoLog);
  glUseProgram(program);
}

void setupPyramid(ShapeInfo *pInfo)
{
    typedef struct {
        GLfloat x, y, z;
        GLubyte red, green, blue;
    } VertexInfo;

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

    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidData), pyramidData, GL_STATIC_DRAW);

    glVertexPointer(3, GL_FLOAT, sizeof(VertexInfo), (GLvoid*)offsetof(VertexInfo, x));
    glColorPointer(3, GL_UNSIGNED_BYTE, sizeof(VertexInfo), (GLvoid*)offsetof(VertexInfo, red));

    pInfo->count = 12;
    pInfo->vboId = vboId;
}

void drawTrianglesAt(double x, double y, double z, double rotyDegrees, double scale, ShapeInfo *pInfo)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(x, y, z - CENTER_Z);
    glRotated(rotyDegrees, 0., 1., 0.);
    glScaled(scale, scale, scale);

    glBindBuffer(GL_ARRAY_BUFFER, pInfo->vboId);
    glDrawArrays(GL_TRIANGLES, 0, pInfo->count);
}

ShapeInfo g_Pyramid;

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
    drawTrianglesAt(cos(angle + 2 * M_PI / 3.), sin(angle + 2 * M_PI / 3.), z, i, 1.5, &g_Pyramid);
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
    setupShaders();
    glutDisplayFunc(onDisplay);
    glutIdleFunc(onDisplay);
    glutKeyboardFunc(onKey);
    glutMainLoop();

    return 0;
}
