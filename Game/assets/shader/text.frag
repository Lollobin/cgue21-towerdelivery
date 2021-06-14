#version 430 core
layout(location = 0) out vec4 color;
//layout(location = 1) out vec4 BrightColor;

in vec2 TexCoords;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
    //BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}