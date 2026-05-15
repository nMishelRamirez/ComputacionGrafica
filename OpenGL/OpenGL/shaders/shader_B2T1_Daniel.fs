#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
in float TexID;

uniform float time;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    vec4 texColor;
    if (TexID < 0.5)
        texColor = texture(texture1, TexCoord);
    else
        texColor = texture(texture2, TexCoord);


    FragColor = texColor;
}
