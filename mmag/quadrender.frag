#version 410
// in variables

in vec2 uv;

// out variables
out vec4 out_color;
//this is 440 layout (binding = 0)
uniform sampler2D quadtex;


void main() {
    out_color = texture(quadtex, uv);
}
