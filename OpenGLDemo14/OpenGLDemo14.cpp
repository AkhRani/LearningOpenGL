/*
 * Demo 14:
 * Added model/view matrix to vertex shader.
 *
 * gl_ModelViewMatrix and associates were deprecated in OpenGL 3 / GLSL 1.3.
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

// #define _USE_MATH_DEFINES
// #include <math.h>
#include <vmath.h>
using vmath::mat4;

// Apparently someone is still using segmented memory qualifiers,
// and windows.h is letting them.
#undef near
#undef far

#define CENTER_Z        6.0f     // Distance from camera
#define DEPTH_OF_FIELD  5.0f

const float PIF();

typedef struct {
    GLsizei count;
    GLuint vboId;
} ShapeInfo;

ShapeInfo g_Pyramid;
GLuint g_progid;
mat4 g_ProjectionMatrix(mat4::identity());

void setupShaders()
{
    GLchar infoLog[4096];
    GLsizei length;

    const GLchar *vertShaderSource[] = {
        "#version 120\n"
        "uniform mat4 ModelViewProject;\n"
        "attribute vec4 vPosition;\n"
        "attribute vec3 vColor;\n"
        "varying vec3 color;\n"
        "void main() {\n"
        "    gl_Position = ModelViewProject * vPosition;"
        "    color = vColor;"
        "}\n"
    };
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, vertShaderSource, NULL);
    glCompileShader(vertShader);
    glGetShaderInfoLog(vertShader, 4096, &length, infoLog);

    const GLchar *fragShaderSource[] = {
        "#version 120\n"
        "varying vec3 color;\n"
        "out vec4 fAnotherColor;\n"
        "out vec4 fColor;\n"
        "void \n"
        "main() {\n"
        // Exercise:  Which one of these actually coloring the pixels?
        "    fAnotherColor = vec4(0, 255, 0, 255);\n"
        "    fColor = vec4(color, 255);\n"
        "}\n"
    };
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, fragShaderSource, NULL);
    glCompileShader(fragShader);
    glGetShaderInfoLog(fragShader, 4096, &length, infoLog);

    g_progid = glCreateProgram();
    glAttachShader(g_progid, vertShader);
    glAttachShader(g_progid, fragShader);
    glBindFragDataLocation(g_progid, 0, "fColor");
    glLinkProgram(g_progid);
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

    GLuint vpos = glGetAttribLocation(g_progid, "vPosition");
    glVertexAttribPointer(vpos, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (GLvoid*)offsetof(VertexInfo, x));
    glEnableVertexAttribArray(vpos);

    GLuint color = glGetAttribLocation(g_progid, "vColor");
    glVertexAttribPointer(color, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexInfo), (GLvoid*)offsetof(VertexInfo, red));
    glEnableVertexAttribArray(color);

    pInfo->count = 12;
    pInfo->vboId = vboId;
}

// The frustum won't be changing, so just set it up once
void setupFrustum(float left, float right, float bottom, float top, float near, float far)
{
    g_ProjectionMatrix = vmath::frustum(left, right, bottom, top, near, far);
}

void drawTrianglesAt(float x, float y, float z, float rotyDegrees, float scale, ShapeInfo *pInfo)
{
    // Since we're setting up our own uniform, we can't use the functions that
    // manipulate the built-in matrix uniforms any more.  vmath to the rescue.

    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
    // glTranslated(x, y, z - CENTER_Z);
    mat4 modelViewMatrix(vmath::translate(x, y, z - CENTER_Z));

    // glRotated(rotyDegrees, 0., 1., 0.);
    modelViewMatrix *= vmath::rotate(rotyDegrees, 0.f, 1.f, 0.f);

    // glScaled(scale, scale, scale);
    modelViewMatrix *= vmath::scale(scale, scale, scale);

    GLint mvp = glGetUniformLocation(g_progid, "ModelViewProject");
    glUniformMatrix4fv(mvp, 1, GL_FALSE, g_ProjectionMatrix * modelViewMatrix);
    glBindVertexArray(pInfo->vboId);
    glDrawArrays(GL_TRIANGLES, 0, pInfo->count);
}

void onDisplay()
{
    static int i = 0;
    if (i < 400) {
        i++;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float z = -i/200.f;
    float angle = i/30.f;
    drawTrianglesAt(cosf(angle), sinf(angle), z, i*3.f, 2.f, &g_Pyramid);

    float angle2 = angle + 2 * float(M_PI) / 3.f;
    drawTrianglesAt(cosf(angle2), sinf(angle2), z, i*1.f, 1.5f, &g_Pyramid);

    float angle3 = angle + 4 * float(M_PI) / 3.f;
    drawTrianglesAt(cosf(angle3), sinf(angle3), z, i*10.f, 1.2f, &g_Pyramid);
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

    GLfloat ratio = 640.0f / 480.0f;
    // glMatrixMode(GL_PROJECTION);
    // glFrustum(-ratio, ratio, -1., 1., CENTER_Z - DEPTH_OF_FIELD/2, CENTER_Z + DEPTH_OF_FIELD/2);
    setupFrustum(-ratio, ratio, -1., 1., CENTER_Z - DEPTH_OF_FIELD/2, CENTER_Z + DEPTH_OF_FIELD/2);

    setupShaders();
    setupPyramid(&g_Pyramid);
    glutDisplayFunc(onDisplay);
    glutIdleFunc(onDisplay);
    glutKeyboardFunc(onKey);
    glutMainLoop();

    return 0;
}
