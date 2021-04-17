#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 modelMatrix;
uniform mat4 vpMatrix;

void main()
{
    FragPos = vec3(modelMatrix * vec4(aPos, 1.0));
    Normal = aNormal;  
    
    gl_Position = vpMatrix * vec4(FragPos, 1.0);
}