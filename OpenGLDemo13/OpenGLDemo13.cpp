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
 *
 * Demo 12:
 * Switched to VBOs
 *
 * Demo 13:
 * Switched from fixed-function pipeline to shader programs.
 */
#include <windows.h>
#include <WinGDI.h>

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/GL.h>
#include <glut.h>

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

ShapeInfo g_Pyramid;

// Must match hard-coded location in vertShaderSource
#define V_POSITION 0

// Must match hard-coded location in vertShaderSource
#define C_POSITION 1

void setupShaders()
{
    GLchar infoLog[4096];
    GLsizei length;

    const GLchar *vertShaderSource[] = {
        "#version 430 core\n"
        "layout(location = 0) in vec4 vPosition;\n"
        "layout(location = 1) in vec3 vColor;\n"
        "out vec3 color;\n"
        "void main() {\n"
        "    gl_Position = vPosition;"
        "    color = vColor;"
        "}\n"
    };
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, vertShaderSource, NULL);

    const GLchar *fragShaderSource[] = {
        "#version 430 core\n"
        "in vec3 color;\n"
        "out vec4 fColor;\n"
        "void \n"
        "main() {\n"
        "    fColor = vec4(color, 255);\n"
        "}\n"
    };
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, fragShaderSource, NULL);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glCompileShader(vertShader);
    glGetShaderInfoLog(vertShader, 4096, &length, infoLog);

    glAttachShader(program, fragShader);
    glCompileShader(fragShader);
    glGetShaderInfoLog(fragShader, 4096, &length, infoLog);

    glLinkProgram(program);
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
        { 0.433f, 0.f, -.25f, 0, 255, 255},
        { 0.0f, 0.75f, 0.f, 255, 0, 255},
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

    glVertexAttribPointer(V_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (GLvoid*)offsetof(VertexInfo, x));
    glEnableVertexAttribArray(V_POSITION);
    glVertexAttribPointer(C_POSITION, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexInfo), (GLvoid*)offsetof(VertexInfo, red));
    glEnableVertexAttribArray(C_POSITION);

    pInfo->count = 12;
    pInfo->vboId = vboId;
}

void drawTrianglesAt(double x, double y, double z, double rotyDegrees, double scale, ShapeInfo *pInfo)
{
    // TODO in Demo 14:
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
    // gluLookAt(-x, -y, -z+CENTER_Z, -x, -y, -z, 0., 1., 0.);
    // glRotated(rotyDegrees, 0., 1., 0.);
    // glScaled(scale, scale, scale);

    glBindVertexArray(pInfo->vboId);
    glDrawArrays(GL_TRIANGLES, 0, pInfo->count);
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
    drawTrianglesAt(cos(angle), sin(angle), depth, i*3., 2., &g_Pyramid);
    drawTrianglesAt(cos(angle + 2 * M_PI / 3.), sin(angle + 2 * M_PI / 3.), depth, i, 1.5, &g_Pyramid);
    drawTrianglesAt(cos(angle + 4 * M_PI / 3.), sin(angle + 4 * M_PI / 3.), depth, i*10., 1.2, &g_Pyramid);
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

    glewInit();
    wglSwapIntervalEXT(1);

    glEnable(GL_DEPTH_TEST);

    // TODO in Demo 14:
    // glMatrixMode(GL_PROJECTION);
    // GLdouble ratio = 640.0f / 480.0f;
    // glFrustum(-ratio, ratio, -1., 1., CENTER_Z - DEPTH_OF_FIELD/2, CENTER_Z + DEPTH_OF_FIELD/2);

    setupShaders();
    setupPyramid(&g_Pyramid);
    glutDisplayFunc(onDisplay);
    glutIdleFunc(onDisplay);
    glutKeyboardFunc(onKey);
    glutMainLoop();

    return 0;
}
