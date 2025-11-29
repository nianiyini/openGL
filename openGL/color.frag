#version 460 core
out vec4 FragColor;

//#include "all_light.glsl"

struct Material {
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;    

    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Direct_Light {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Point_Light {
    vec3 position;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

struct Spotlight {
    vec3  position;
    vec3  direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

in vec3 Normal;
in vec3 FragPos; 
in vec2 TexCoords;

uniform sampler2D ourTexture;
//uniform vec3 objectColor;
//uniform vec3 lightColor;
//uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform Material material;
uniform Light l_light;
uniform Direct_Light d_light;
uniform Point_Light p_light;
uniform Spotlight s_light;

void main()
{    
    // ambient
    vec3 s_ambient = p_light.ambient *  vec3(texture(material.diffuse, TexCoords));

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(p_light.position- FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 s_diffuse = p_light.diffuse * (diff * vec3(texture(material.diffuse, TexCoords)));
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 s_specular = p_light.specular * (spec * vec3(texture(material.specular, TexCoords)));  
        
    float distance    = length(p_light.position - FragPos);
    float attenuation = 1.0 / (p_light.constant + p_light.linear * distance + 
    p_light.quadratic * (distance * distance));
    s_ambient  *= attenuation; 
    s_diffuse  *= attenuation;
    s_specular *= attenuation;

    /////////////////////////////////////////////////////////////////////////
    lightDir = normalize(s_light.position - FragPos);
    
    // check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-s_light.direction)); 
    
    if(theta > s_light.cutOff) // remember that we're working with angles as cosines instead of degrees so a '>' is used.
    {    
        // ambient
        vec3 ambient = s_light.ambient * texture(material.diffuse, TexCoords).rgb;
        
        // diffuse 
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = s_light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
        
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = s_light.specular * spec * texture(material.specular, TexCoords).rgb;  
        
        // attenuation
        float distance    = length(s_light.position - FragPos);
        float attenuation = 1.0 / (s_light.constant + s_light.linear * distance + s_light.quadratic * (distance * distance));    

        diffuse   *= attenuation;
        specular *= attenuation;   
    }
    else 
    {
        // else, use ambient s_light so scene isn't completely dark outside the spotlight.
        FragColor = vec4(s_light.ambient * texture(material.diffuse, TexCoords).rgb, 1.0);
    }

    vec3 ambient  = s_ambient; 
    vec3 diffuse  = s_diffuse;
    vec3 specular = s_specular;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}