#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec3 fragPos;
in vec3 normal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor = vec3(1.0,1.0,1.0);

//shadow stuff
in vec4 FragPosLightSpace;
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow - adjust for peter panning
    float shadow = currentDepth - 0.01 > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main()
{
        // ambient
        float ambientStrength = 0.2;
        vec3 ambient = ambientStrength * lightColor;
          
        // diffuse
        vec3 norm = normalize(normal);
        vec3 lightDir = normalize(lightPos - fragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
        
        // specular
        float specularStrength = 0.3;
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor;
            
        // calculate shadow
        float shadow = ShadowCalculation(FragPosLightSpace);
        //shadow = 1.0;
        vec3 litColor = (ambient + (1.0 - shadow) * (diffuse + specular)) * vertexColor;
        FragColor = vec4(litColor, 1.0);
}
