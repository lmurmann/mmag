#ifndef LUM_GLUTIL_H
#define LUM_GLUTIL_H
void printProgramInfoLog(GLuint program_id);
void printShaderInfoLog(GLuint shader_id);
std::string PrintOpenGLInfo();
int get_shader_src(const std::string& shaderpath, std::string* shader_src);
GLuint compile_shaders(const std::string& vshader, const std::string& fshader);
#endif