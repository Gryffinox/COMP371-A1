#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec3 fragPos;
in vec3 normal;
in vec3 textureCoords;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor = vec3(1.0,1.0,1.0);

uniform float shininess = 32;

uniform sampler2D tex;
uniform bool textureOn;
uniform sampler2D emissionMap;
uniform bool glowOn;
uniform float intensity = 1;
uniform bool colorOn = true;

void main()
{
    // ambient
        float ambientStrength = 0.5;
        vec3 ambient = ambientStrength * lightColor;
          
        // diffuse
        vec3 norm = normalize(normal);
        vec3 lightDir = normalize(lightPos - fragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
        
        // specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = specularStrength * spec * lightColor;
    
        vec3 litColor = (ambient + diffuse + specular ) * vertexColor;
        
    if(textureOn)
    {
        FragColor = texture(tex, vec2(textureCoords.x, textureCoords.y)) *  vec4(litColor, 1.0);
        if(glowOn)
        {
            FragColor += intensity * (texture(emissionMap, vec2(textureCoords.x, textureCoords.y)) *  vec4(vertexColor, 1.0));
        }
    } else {
        FragColor = vec4(litColor, 1.0);
    }
        
}
