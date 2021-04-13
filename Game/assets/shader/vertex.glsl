//Vertex Shader

#version 430

in vec3 position;

//layout(location = 1) uniform mat4 tMatrix;	//transformation matrix
layout(location = 2) uniform mat4 vpMatrix;	//view-projection matrix


void main(){
	//gl_Position = vpMatrix*tMatrix*vec4(position,1);
	gl_Position = vpMatrix*vec4(position,1);
}