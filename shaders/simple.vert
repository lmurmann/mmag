#version 410

layout(location=0) in vec3 in_xy;
layout(location=1) in vec2 in_uv;

out vec2 texcoord;

void main(){
    texcoord = in_uv;
    gl_Position = vec4(in_xy.x, in_xy.y, in_xy.z, 1);
}
