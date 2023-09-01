#version 330 core

layout (location = 0) in vec4 vertex; // 2 pos, 2 tex

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec2 texture_coordinates;

void main()
{
    gl_Position = projection * view * model * vec4(vertex.xy, 0.0, 1.0);
    texture_coordinates = vertex.zw;
}
