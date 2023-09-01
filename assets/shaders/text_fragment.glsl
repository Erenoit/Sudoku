#version 330 core

in vec2 texture_coordinates;

uniform sampler2D text;
uniform vec4 text_color;

out vec4 color;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, texture_coordinates).r);
    color = text_color * sampled;
}
