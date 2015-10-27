#version 410
// in variables
in vec2 texcoord;

// out variables
out vec4 out_color;

void main() {
    out_color = vec4(1, texcoord.x, texcoord.y, 1);
}
