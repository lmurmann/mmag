#include <OpenGL/gl3.h>
#include <cstdio>
#include <fstream>
#include <cassert>

void printProgramInfoLog(GLuint program) {
    GLsizei len;
    const int bufsz = 1024;
    GLchar buff[bufsz];
    glGetProgramInfoLog(program, bufsz, &len, buff);
    fprintf(stderr, "%s", buff);
}

void printShaderInfoLog(GLuint shader) {
    GLsizei len;
    const int bufsz = 1024;
    GLchar buff[bufsz];
    glGetShaderInfoLog(shader, bufsz, &len, buff);
    fprintf(stderr, "%s", buff);
}

std::string PrintOpenGLInfo()  {
    GLint major, minor;
    glGetIntegerv( GL_MAJOR_VERSION, &major );
    glGetIntegerv( GL_MINOR_VERSION, &minor );
    const GLubyte *renderer    = glGetString( GL_RENDERER );
    const GLubyte *vendor      = glGetString( GL_VENDOR );
    const GLubyte *version     = glGetString( GL_VERSION );
    const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );
    std::string ret;
    char buff[400];
    sprintf(buff, "GL Vendor : %s\n", vendor );
    ret += buff;
    sprintf(buff, "GL Renderer : %s\n", renderer );
    ret += buff;
    sprintf(buff, "GL Version (string) : %s\n", version );
    ret += buff;
    sprintf(buff, "GL Version (integer) : %d.%d\n", major, minor );
    ret += buff;
    sprintf(buff, "GLSL Version : %s\n", glslVersion );
    ret += buff;
    return ret;
}

int get_shader_src(const std::string& shaderpath, std::string* shader_src) {
    std::ifstream ifst(shaderpath, std::ifstream::in);
    if (!ifst.is_open()) {
        fprintf(stderr, "Cannot open file %s\n", shaderpath.c_str());
        return -1;
    }
    shader_src->clear();
    while(!ifst.eof()) {
        char g = ifst.get();
        if (g != EOF) {
            shader_src->push_back(g);
        }
    }
    return 0;
}

GLuint compile_shaders(const std::string& vshader, const std::string& fshader) {
    /* Shaders and Program */
    GLuint vshader_id = glCreateShader(GL_VERTEX_SHADER);
    if (vshader_id == 0) {
        fprintf(stderr, "could not generate vshader object\n");
        return 0;
    }
    GLuint fshader_id = glCreateShader(GL_FRAGMENT_SHADER);
    if (fshader_id == 0) {
        fprintf(stderr, "could not generate fshader object\n");
        return 0;
    }
    const GLchar* vsrcp = vshader.c_str();
    const GLchar* fsrcp = fshader.c_str();
    const GLint vshader_len = vshader.size();
    const GLint fshader_len = fshader.size();
    glShaderSource(vshader_id, 1, &vsrcp, &vshader_len);
    glShaderSource(fshader_id, 1, &fsrcp, &fshader_len);
    if (glGetError()) {
        fprintf(stderr, "problem during shader upload\n");
        return 0;
    }

    glCompileShader(vshader_id);
    GLint compile_status;
    glGetShaderiv(vshader_id, GL_COMPILE_STATUS, &compile_status);
    if (!compile_status) {
        fprintf(stderr, "vshader compilation error\n");
        fprintf(stderr, "vshader log\n");
        printShaderInfoLog(vshader_id);
        return 0;
    }
    glCompileShader(fshader_id);
    glGetShaderiv(fshader_id, GL_COMPILE_STATUS, &compile_status);
    if (!compile_status) {
        fprintf(stderr, "fshader compilation error\n");
        fprintf(stderr, "fshader log:\n");
        printShaderInfoLog(fshader_id);
        return 0;
    }

    GLuint program_id = glCreateProgram();
    if (program_id == 0) {
        fprintf(stderr, "could not create program object\n");
        return 0;
    }

    glAttachShader(program_id, vshader_id);
    glAttachShader(program_id, fshader_id);
    glDeleteShader(vshader_id);
    glDeleteShader(fshader_id);

    glLinkProgram(program_id);
    GLint link_status;
    glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);
    if (!link_status) {
        fprintf(stderr, "linking program failed. Check glGetProgramInfoLog for details.\n");
        printProgramInfoLog(program_id);
        glDeleteProgram(program_id);
        return 0;
    }
    return program_id;
}
