#version 330 core

struct PointLight
{
    vec3 position;
    float intensity;
    float range;
    vec3 color;
};

struct DirectionalLight
{
    vec3 direction;
    vec3 color;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutoff;
    float intensity;
    float range;
    vec3 color;
};

#define MAX_POINT_LIGHTS 32
uniform int u_nPointLights;
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];

#define MAX_DIRECTIONAL_LIGHTS 4
uniform int u_nDirectionalLights;
uniform DirectionalLight u_directionalLights[MAX_DIRECTIONAL_LIGHTS];

#define MAX_SPOT_LIGHTS 32
uniform int u_nSpotLights;
uniform SpotLight u_spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D u_Texture;
uniform vec3 u_viewPos;
uniform vec3 u_blendColor;
uniform vec2 u_uvOffset;

in vec3 Pos;
in vec3 Normal;
in vec2 TexCoord;
out vec4 FragColor;


vec3 CalcPointLight(
    PointLight light,
    vec3 normal,
    vec3 fragPos)
{
    float distance    = length(light.position - fragPos);
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 viewDir = normalize(u_viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal); 
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float attenuation = 1.0 / (1.0 + (distance / light.range) * (distance / light.range));

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = 0.8 * pow(max(dot(viewDir, halfwayDir), 0.0), 64);

    return light.color * light.intensity * attenuation * (spec + diff);
}

vec3 CalcDirectionalLight(
    DirectionalLight light,
    vec3 normal,
    vec3 fragPos)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 viewDir = normalize(u_viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = 0.8 * pow(max(dot(viewDir, halfwayDir), 0.0), 64);

    return light.color * (spec + diff);
}

vec3 CalcSpotLight(
    SpotLight light,
    vec3 normal,
    vec3 fragPos)
{
    float distance = length(light.position - fragPos);
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 viewDir = normalize(u_viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal); 
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float cos_theta = dot(lightDir, normalize(-light.direction));
    float epsilon   = -light.cutoff * 0.05;
    float intensity = clamp(-(cos_theta - light.cutoff * 0.95) / epsilon, 0.0, 1.0);
    intensity = intensity * intensity;
    
    float attenuation = 1.0 / (1.0 + (distance / light.range) * (distance / light.range));

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = 0.8 * pow(max(dot(viewDir, halfwayDir), 0.0), 64);

    return light.color * light.intensity * intensity * attenuation * (spec + diff);
}


void main()
{
    vec3 albedo = vec3(texture(u_Texture, TexCoord + u_uvOffset));
    vec3 result = vec3(0.0f);

    for (int i=0; i<u_nPointLights; i++)
    {
        result += albedo * CalcPointLight(u_pointLights[i], Normal, Pos);
    }
    for (int i=0; i<u_nDirectionalLights; i++)
    {
        result += albedo * CalcDirectionalLight(u_directionalLights[i], Normal, Pos);
    }
    for (int i=0; i<u_nSpotLights; i++)
    {
        result += albedo * CalcSpotLight(u_spotLights[i], Normal, Pos);
    }

    FragColor = vec4(result * u_blendColor, 1.0);
}