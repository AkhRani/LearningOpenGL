/*
 * Demo 13:
 * Switched from built-in attributes to user-defined attributes.
 *
 * gl_Vector, gl_Color, etc. were deprecated in OpenGL 3 / GLSL 1.3,
 * in favor of user-defined attributes.
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

ShapeInfo g_Pyramid;
GLuint g_progid;    // Shader Program ID

void setupShaders()
{
    GLchar infoLog[4096];
    GLsizei length;


    const GLchar *vertShaderSource[] = {
      "#version 120\n"
      "attribute vec4 vPosition;"
      "attribute vec3 vColor;"
      "void main() {\n"
      "    gl_Position = gl_ModelViewProjectionMatrix * vPosition;"
      "    gl_FrontColor = vec4(vColor, 1.0);"
      "}\n"
    };
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, vertShaderSource, NULL);
    glCompileShader(vertShader);
    glGetShaderInfoLog(vertShader, 4096, &length, infoLog);

    const GLchar *fragShaderSource[] = {
        "#version 120\n"
        "void main() {\n"
        "    gl_FragColor = gl_Color;\n"
        "}\n"
    };
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, fragShaderSource, NULL);
    glCompileShader(fragShader);
    glGetShaderInfoLog(fragShader, 4096, &length, infoLog);

    g_progid = glCreateProgram();
    glAttachShader(g_progid, vertShader);
    glAttachShader(g_progid, fragShader);
    glLinkProgram(g_progid);
    glGetProgramInfoLog(g_progid, 5096, &length, infoLog);
    glUseProgram(g_progid);
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

    // Instead of calling glVertexPointer and glColorPointer, we use the
    // general purpose glVertexAttribPointer to set the offsets for our user-defined attributes.
    GLuint vpos = glGetAttribLocation(g_progid, "vPosition");
    glVertexAttribPointer(vpos, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (GLvoid*)offsetof(VertexInfo, x));
    glEnableVertexAttribArray(vpos);

    GLuint color = glGetAttribLocation(g_progid, "vColor");
    glVertexAttribPointer(color, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexInfo), (GLvoid*)offsetof(VertexInfo, red));
    glEnableVertexAttribArray(color);

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

    glewInit();
    wglSwapIntervalEXT(1);

    glEnable(GL_DEPTH_TEST);

    // We no longer call glEnableClientState for GL_VERTEX_ARRAY and GL_COLOR_ARRAY

    glMatrixMode(GL_PROJECTION);
    GLdouble ratio = 640.0f / 480.0f;
    glFrustum(-ratio, ratio, -1., 1., CENTER_Z - DEPTH_OF_FIELD/2, CENTER_Z + DEPTH_OF_FIELD/2);

    setupShaders();
    setupPyramid(&g_Pyramid);
    glutDisplayFunc(onDisplay);
    glutIdleFunc(onDisplay);
    glutKeyboardFunc(onKey);
    glutMainLoop();

    return 0;
}
