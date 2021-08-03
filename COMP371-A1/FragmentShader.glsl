#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec3 fragPos;
in vec3 normal;
in vec3 textureCoords;

in vec4 FragPosLightSpace;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor = vec3(1.0,1.0,1.0);

//texture stuff
uniform float shininess = 32;
uniform float intensity = 1;

uniform bool textureOn;
uniform bool glowOn;
uniform bool colorOn = true;

uniform sampler2D tex;
uniform sampler2D emissionMap;

//shadow stuff
uniform bool drawShadows;
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
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float bias = max(0.25 * (1.0 - dot(norm, lightDir)), 0.1);
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main()
{
        // ambient
        float ambientStrength = 0.4;
        vec3 ambient = ambientStrength * lightColor;
          
        // diffuse
        vec3 norm = normalize(normal);
        vec3 lightDir = normalize(lightPos - fragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
        
        // specular
        float specularStrength = 0.4;
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = specularStrength * spec * lightColor;
        
        // calculate shadow
        float shadow = 0.0;
        if(drawShadows){
            shadow = ShadowCalculation(FragPosLightSpace);
        }
        else{
            shadow = 0.0;
        }

        //calculate overall lighting through phong lighting and shadow
        vec3 litColor = (ambient + (1.0 - shadow) * (diffuse + specular));
        
        //color settings
        if (colorOn){
            litColor = litColor * vertexColor;
        }
        //texture settings
        if(textureOn)
        {
            FragColor = texture(tex, vec2(textureCoords.x, textureCoords.y)) *  vec4(litColor, 1.0);   
        } 
        else {
            FragColor = vec4(litColor, 1.0);
        }
        //glow emission map for metal cubes
        if(glowOn)
        {
            FragColor += intensity * (texture(emissionMap, vec2(textureCoords.x, textureCoords.y)) *  vec4(vertexColor, 1.0));
        }   
}
