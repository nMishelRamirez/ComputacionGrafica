#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aTexID;

out vec3 ourColor;
out vec2 TexCoord;
out float TexID;

uniform float time;
uniform mat4 transform;

void main()
{
    float moveX = sin(time) / 5;
    gl_Position = transform * vec4(aPos.x + moveX, aPos.y, aPos.z, 1.0);
    ourColor = aColor;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    TexID = aTexID;
}
