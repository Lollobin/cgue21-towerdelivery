#version 430 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 color;
uniform float ambientStrength;

void main()
{
	vec3 lightColor = vec3(1,1,1);

    // ambient
    vec3 ambient = 1 * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
            
    vec3 result = (ambient + diffuse) * color;
    FragColor = vec4(result, 1.0);
} 