#version 330 core
const float PI = 3.14159265359;

//////////////////////////////
// CAMERA DATA
//////////////////////////////
uniform vec3 u_viewPos;


//////////////////////////////
// LIGHTING DATA
//////////////////////////////
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

#define MAX_POINT_LIGHTS 8
uniform int u_nPointLights;
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];

#define MAX_DIRECTIONAL_LIGHTS 8
uniform int u_nDirectionalLights;
uniform DirectionalLight u_directionalLights[MAX_DIRECTIONAL_LIGHTS];

#define MAX_SPOT_LIGHTS 8
uniform int u_nSpotLights;
uniform SpotLight u_spotLights[MAX_SPOT_LIGHTS];

//////////////////////////////
// PBR SURFACE MODEL
//////////////////////////////
struct PBRSurfaceData
{
    vec3 position;
    vec3 normal;
    vec3 albedo;
    float metallic;
    float roughness;
};

/* Trowbridge-Reitz GGX model */
float NormalDistribution_GGX(float cosNH, float roughness)
{
    float roughnessSquared = roughness * roughness;
    float denominator = cosNH * cosNH * (roughnessSquared - 1.0) + 1.0;
    return roughnessSquared / (PI * denominator * denominator);
}

/* Height Correlated Smith View */
float View_HCSmith(float cosNL, float cosNV, float roughness)
{
    float Vv = cosNL * (cosNV * (1.0 - roughness) + roughness);
    float Vl = cosNV * (cosNL * (1.0 - roughness) + roughness);

    return 0.5 / (Vv + Vl);
}

/* Schlick approximation */
vec3 Fresnel_Schlick(float cosVH, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosVH, 5.0);
}

vec3 BRDF(
    vec3 viewDir,
    vec3 lightDir,
    vec3 halfwayDir,
    PBRSurfaceData surface)
{
    float cosNH = clamp(dot(surface.normal, halfwayDir), 1.0e-6, 1.0);
    float cosNL = clamp(dot(surface.normal, lightDir), 1.0e-6, 1.0);
    float cosNV = clamp(dot(surface.normal, viewDir), 1.0e-6, 1.0);
    float cosVH = clamp(dot(viewDir, halfwayDir), 1.0e-6, 1.0);

    vec3 F0 = mix(vec3(0.04), surface.albedo, surface.metallic);
    vec3 diffuseColor =  (1.0 - surface.metallic) * surface.albedo;

    float D = NormalDistribution_GGX(cosNH, surface.roughness);
    float V = View_HCSmith(cosNL, cosNV, surface.roughness);
    vec3 F = Fresnel_Schlick(cosVH, F0);

    vec3 specular = D * V * F;
    vec3 diffuse = (vec3(1.0) - F) * diffuseColor / PI;

    vec3 f = diffuse + specular;

    return f * cosNL * 2;
}


//////////////////////////////
// MATERIAL DATA
//////////////////////////////
uniform vec3 u_albedo;
uniform float u_roughness;
uniform float u_metalic;

in vec3 Pos;
in vec3 Normal;
in vec2 TexCoord;
out vec4 FragColor;


void main()
{
    // Specify surface
    PBRSurfaceData surface;
    surface.position = Pos;
    surface.normal = normalize(Normal);
    surface.albedo = u_albedo;
    surface.metallic = u_metalic;
    surface.roughness = u_roughness * u_roughness;

    // Accumulate lighting contributions
    vec3 result = vec3(0.0f);
    for (int i=0; i<u_nPointLights; i++) {}
    for (int i=0; i<u_nDirectionalLights; i++) {}
    for (int i=0; i<u_nSpotLights; i++) {}

    for (int i=0; i<3; i++)
    {
        float x = (-1.0f + i) * 20.0;
        vec3 lightPos = vec3(x, 0.0, 50.0);
        
        float distance = length(lightPos - surface.position);
        vec3 viewDir = normalize(u_viewPos - surface.position);
        vec3 lightDir = normalize(lightPos - surface.position);
        vec3 halfwayDir = normalize(lightDir + viewDir);

        result += BRDF(viewDir, lightDir, halfwayDir, surface);
    }

    result += vec3(0.03) * surface.albedo;
    

    result = result / (result + vec3(1.0));
    result = pow(result, vec3(1.0/2.2)); 

    FragColor = vec4(result, 1.0);
}