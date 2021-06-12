#version 430 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec4 particlecolor;

// Ouput data

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 BrightColor;

uniform sampler2D myTextureSampler;

void main(){
	// Output color = color of the texture at the specified UV
	color = texture( myTextureSampler, UV ) * particlecolor;

	float brightness = dot(vec3(color), vec3(0.2126, 0.7152, 0.0722));

    if(brightness > 1.0)
        BrightColor = color;
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}