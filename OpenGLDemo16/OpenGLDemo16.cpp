/*
 * Demo 16:
 * Setting up custom sampler.
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
GLint g_MatrixUniform, g_SamplerUniform;
mat4 g_ProjectionMatrix(mat4::identity());

#define USE_BLOCKY_SAMPLER

#define BLOCKY_SAMPLER 1

// Must match hard-coded vPosition location in vertShaderSource
#define V_POSITION 0

// Must match hard-coded location in vertShaderSource
#define C_POSITION 1

// Must match hard-coded vTexture location in vertShaderSource
#define T_POSITION 2

void setupShaders()
{
    GLchar infoLog[4096];
    GLsizei length;

    const GLchar *vertShaderSource[] = {
        "#version 430 core\n"
        "uniform mat4 ModelViewProject;\n"
        "layout(location = 0) in vec4 vPosition;\n"
        "layout(location = 1) in vec3 vColor;\n"
        "layout(location = 2) in vec2 vTexture;\n"
        "out vec3 color;\n"
        "out vec2 vs_tex_coord;\n"
        "void main() {\n"
        "    gl_Position = ModelViewProject * vPosition;\n"
        "    vs_tex_coord = vTexture;\n"
        "    color = vColor;\n"
        "}\n"
    };
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, vertShaderSource, NULL);

    const GLchar *fragShaderSource[] = {
        "#version 430 core\n"
        "uniform sampler2D tex;\n"
        "in vec3 color;\n"
        "in vec2 vs_tex_coord;\n"
        "out vec4 fColor;\n"
        "void \n"
        "main() {\n"
        "    vec4 texColor = texture(tex, vs_tex_coord);\n"
        "    fColor = vec4(color, 0) * (1 - texColor.a) + texColor;\n"
        "}\n"
    };
    // Try one of these lines in the fragment shader for a different effect
//        "    fColor = vec4(vs_tex_coord, 0., 255);\n"
//        "    fColor = vec4(color, 255) + texColor;\n"
//        "    fColor = texColor;\n"
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
    g_MatrixUniform = glGetUniformLocation(program, "ModelViewProject");
    g_SamplerUniform = glGetUniformLocation(program, "tex");
}


// Convert a simple bitmap (one bit per pixel) into an RGBA bitmap (four bytes per pixel)
GLubyte* BuildMonochromeBitmap(const GLubyte* bits, int width, int height, GLubyte red, GLubyte green, GLubyte blue)
{
    GLubyte* retval = (GLubyte *)malloc(width * height * 4);
    if (retval) {
        GLubyte* ptr = retval;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width/8; x++) {
                GLubyte next8 = *bits++;
                for (int mask = 128; mask > 0; mask >>= 1) {
                    if (next8 & mask) {
                        *ptr++ = red;
                        *ptr++ = green;
                        *ptr++ = blue;
                        *ptr++ = 255;
                    }
                    else {
                        *ptr++ = 0;
                        *ptr++ = 0;
                        *ptr++ = 0;
                        *ptr++ = 0;
                    }
                }
            }
        }
    }
    return retval;
}

#define BITMAP_WIDTH 16
#define BITMAP_HEIGHT 16
#define BIT_BYTES ((BITMAP_WIDTH / 8) * BITMAP_HEIGHT)

void setupTextures()
{
    // smiley face
    GLubyte bits[BIT_BYTES] = {
        0x00, 0x00,
        0x00, 0x00,
        0x07, 0xE0,
        0x08, 0x10,
        0x10, 0x08,
        0x20, 0x04,
        0x44, 0x22,
        0x40, 0x02,
        0x40, 0x02,
        0x40, 0x02,
        0x42, 0x42,
        0x23, 0xc4,
        0x10, 0x08,
        0x0c, 0x30,
        0x03, 0xc0,
        0x00, 0x00,
    };

    GLubyte* data = BuildMonochromeBitmap(bits, BITMAP_WIDTH, BITMAP_HEIGHT, 255, 0, 0);
    GLuint texture = 0;
    if (data) {
        glGenTextures(1, &texture);
        if (texture) {
            glBindTexture(GL_TEXTURE_2D, texture);
#ifdef USE_BLOCKY_SAMPLER
            // Only need one mipmap level for nearest sampling
            glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, BITMAP_WIDTH, BITMAP_HEIGHT);
#else
            // Need complete mipmaps for default (linear) sampling
            glTexStorage2D(GL_TEXTURE_2D, 4, GL_RGBA8, BITMAP_WIDTH, BITMAP_HEIGHT);
#endif
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, BITMAP_WIDTH, BITMAP_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, data);
#ifndef USE_BLOCKY_SAMPLER
            glGenerateMipmap(GL_TEXTURE_2D);
#endif
        }
        else {
            glDeleteTextures(1, &texture);
            texture = 0;
        }
        free(data);
    }

#ifdef USE_BLOCKY_SAMPLER
    if (texture) {
        GLuint sampler;
        glGenSamplers(1, &sampler);
        if (sampler) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture);
            glBindSampler(BLOCKY_SAMPLER, sampler);
            glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glUniform1i(g_SamplerUniform, BLOCKY_SAMPLER);
        }
    }
#endif
}

void setupPyramid(ShapeInfo *pInfo)
{
    typedef struct {
        GLfloat x, y, z;
        GLubyte red, green, blue;
        GLfloat texU, texV;
    } VertexInfo;

    GLuint vboId(0);

    static const VertexInfo pyramidData[] = {
        // Bottom
        { 0.0f, 0.f, .5f, 255, 0, 0, 0.f, 0.f},
        { 0.433f, 0.f, -.25f, 255, 0, 0, 0.f, 1.f},
        { -0.433f, 0.f, -.25f, 255, 0, 0, 1.f, 1.f},
        // Side 1
        { -0.433f, 0.f, -.25f, 0, 0, 255, 0.f, 0.f},
        { 0.433f, 0.f, -.25f, 0, 255, 255, 1.f, 0.f},
        { 0.0f, 0.75f, 0.f, 255, 0, 255, 1.f, 1.f},
        // Side 2
        { -0.433f, 0.f, -.25f, 255, 255, 0, 0.f, 0.f},
        { 0.0f, 0.f, .5f, 255, 255, 0, 0.f, 1.f},
        { 0.0f, 0.75f, 0.f, 255, 255, 0, 1.f, 1.f},
        // Side 3
        { 0.0f, 0.f, .5f, 0, 255, 0, 4.f, 4.f},
        { 0.0f, 0.75f, 0.f, 0, 255, 0, 2.f, 0.f},
        { 0.433f, 0.f, -.25f, 0, 255, 0, 0.f, 4.f},
    };

    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidData), pyramidData, GL_STATIC_DRAW);

    glVertexAttribPointer(V_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (GLvoid*)offsetof(VertexInfo, x));
    glEnableVertexAttribArray(V_POSITION);
    glVertexAttribPointer(C_POSITION, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexInfo), (GLvoid*)offsetof(VertexInfo, red));
    glEnableVertexAttribArray(C_POSITION);
    glVertexAttribPointer(T_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (GLvoid*)offsetof(VertexInfo, texU));
    glEnableVertexAttribArray(T_POSITION);

    pInfo->count = 12;
    pInfo->vboId = vboId;
}

void setupFrustum(float left, float right, float bottom, float top, float near, float far)
{
    g_ProjectionMatrix = vmath::frustum(left, right, bottom, top, near, far);
}

void drawTrianglesAt(float x, float y, float z, float rotyDegrees, float scale, ShapeInfo *pInfo)
{
    mat4 modelViewMatrix(vmath::translate(x, y, z - CENTER_Z));
    modelViewMatrix *= vmath::rotate(rotyDegrees, 0.f, 1.f, 0.f);
    modelViewMatrix *= vmath::scale(scale, scale, scale);

    glUniformMatrix4fv(g_MatrixUniform, 1, GL_FALSE, g_ProjectionMatrix * modelViewMatrix );
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

//    float angle2 = angle + 2 * float(M_PI) / 3.f;
//    drawTrianglesAt(cosf(angle2), sinf(angle2), z, i*1.f, 1.5f, &g_Pyramid);

//    float angle3 = angle + 4 * float(M_PI) / 3.f;
//    drawTrianglesAt(cosf(angle3), sinf(angle3), z, i*10.f, 1.2f, &g_Pyramid);
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
    wglSwapIntervalEXT(1);	// vsync

    glEnable(GL_DEPTH_TEST);

    GLfloat ratio = 640.0f / 480.0f;
    setupFrustum(-ratio, ratio, -1., 1., CENTER_Z - DEPTH_OF_FIELD/2, CENTER_Z + DEPTH_OF_FIELD/2);

    setupShaders();
    setupPyramid(&g_Pyramid);
    setupTextures();
    glutDisplayFunc(onDisplay);
    glutIdleFunc(onDisplay);
    glutKeyboardFunc(onKey);
    glutMainLoop();

    return 0;
}