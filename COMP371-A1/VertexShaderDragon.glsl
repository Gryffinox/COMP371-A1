#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexture;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);
uniform mat4 normalVectorRotation = mat4(1.0);

//shadow stuff
uniform mat4 lightSpaceMatrix;
out vec4 fragmentPositionLightSpace;

out vec3 vertexColor;
out vec3 fragPos;
out vec3 normal;
out vec2 textureCoords;

void main()
{
   vertexColor = vec3(.2, .2, .2);
   fragPos = vec3(worldMatrix * vec4(aPos, 1.0));
   normal = vec3(normalVectorRotation * vec4(aNorm, 1.0));
   textureCoords = aTexture;
   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;
   fragmentPositionLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);
   gl_Position =  modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

