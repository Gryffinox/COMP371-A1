#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec3 fragPos;
in vec3 normal;
in vec2 textureCoords;

uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 lightColor = vec3(1.0,1.0,1.0);

//testure stuff
uniform float shininess = 32;

uniform sampler2D tex;
uniform bool textureOn = false;
uniform sampler2D emissionMap;
uniform bool glowOn = false;
uniform float intensity = 1;
uniform bool colorOn = true;


//shadow stuff
in vec4 FragPosLightSpace;
uniform sampler2D shadowMap;
uniform bool drawShadows = false;

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
    float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);
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
        float specularStrength = 0.2;
        vec3 viewDir = normalize(cameraPos - fragPos);
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
        vec3 litColor = (ambient + (1.0 - shadow) * (diffuse + specular));
        
        //texture and color settings
        if (colorOn){
            litColor = litColor * vertexColor;
        }
        if(textureOn)
        {
            FragColor = texture(tex, textureCoords) *  vec4(litColor, 1.0);   
        } 
        else {
            FragColor = vec4(litColor, 1.0);
        }
        if(glowOn)
        {
            FragColor += intensity * (texture(emissionMap, textureCoords) *  vec4(vertexColor, 1.0));
        }   
}
