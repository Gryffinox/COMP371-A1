#version 330 core
in vec3 vertexColor;
out vec4 FragColor;

uniform vec3 lightcolor = vec3(1.0,1.0,1.0);

void main()
{
    FragColor = vec4(vertexColor * lightcolor, 1.0f);
}
