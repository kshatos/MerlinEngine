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
    float radiantFlux;
    float range;
    vec3 color;
};

struct DirectionalLight
{
    vec3 direction;
    float irradiance;
    vec3 color;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutoffAngle;
    float falloffRatio;
    float radiantIntensity;
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

    return f * cosNL;
}

//////////////////////////////
// LIGHTING MODEL
//////////////////////////////
float PunctualLightAttenuation(float separation, float range)
{
    float separation2 = separation * separation;
    float relativeRange = min(1.0, separation / range);
    float relativeRange4 = relativeRange * relativeRange * relativeRange * relativeRange;

    return 1.0 / (separation2 + 1.0e-6) * (1.0 - relativeRange4);
}

float SpotLightAttenuation(
    SpotLight light,
    vec3 lightDir)
{
    float cosDL = dot(-light.direction, lightDir);
    float cosTMin = cos(light.cutoffAngle * (1.0 - light.falloffRatio));
    float cosTMax = cos(light.cutoffAngle);
    
    if (cosDL >= cosTMin)
        return 1.0;
    if (cosDL <= cosTMax)
        return 0.0;
    return smoothstep(0.0, 1.0, (cosDL - cosTMax) / (cosTMin - cosTMax));
}

vec3 PointLightReflectedRadiance(
    PointLight light,
    PBRSurfaceData surface)
{
    float separation = length(light.position - surface.position);
    vec3 viewDir = normalize(u_viewPos - surface.position);
    vec3 lightDir = normalize(light.position - surface.position);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    vec3 f = BRDF(viewDir, lightDir, halfwayDir, surface);
    vec3 L0 = light.color * light.radiantFlux / (4.0 * PI);
    float A = PunctualLightAttenuation(separation, light.range);

    return L0 * A * f;
}

vec3 DirectionalLightReflectedRadiance(
    DirectionalLight light,
    PBRSurfaceData surface)
{
    vec3 viewDir = normalize(u_viewPos - surface.position);
    vec3 lightDir = -normalize(light.direction);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    vec3 f = BRDF(viewDir, lightDir, halfwayDir, surface);
    vec3 L0 = light.color * light.irradiance ;

    return f * L0;
}

vec3 SpotLightReflectedRadiance(
    SpotLight light,
    PBRSurfaceData surface)
{
    float separation = length(light.position - surface.position);
    vec3 viewDir = normalize(u_viewPos - surface.position);
    vec3 lightDir = normalize(light.position - surface.position);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    vec3 f = BRDF(viewDir, lightDir, halfwayDir, surface);
    vec3 L0 = light.color * light.radiantIntensity / (4.0 * PI);
    float A = PunctualLightAttenuation(separation, light.range);
    float l = SpotLightAttenuation(light, lightDir);

    return L0 * A * l * f;
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
    for (int i = 0; i < u_nPointLights; i++)
    {
        result += PointLightReflectedRadiance(u_pointLights[i], surface);
    }
    for (int i=0; i < u_nDirectionalLights; i++)
    {
        result += DirectionalLightReflectedRadiance(u_directionalLights[i], surface);
    }
    for (int i = 0; i < u_nSpotLights; i++)
    {
        result += SpotLightReflectedRadiance(u_spotLights[i], surface);
    }

    result += vec3(0.03) * surface.albedo;
    
    // HDR Tonemap and gamma correction
    result = result / (result + vec3(1.0));
    result = pow(result, vec3(1.0/2.2)); 

    FragColor = vec4(result, 1.0);
}