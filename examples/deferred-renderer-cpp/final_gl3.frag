#version 140

uniform sampler2D color;
uniform sampler2D position;
uniform sampler2D normal;
uniform vec2 winSize;

out vec4 fragColor;

struct PointLight
{
    vec3 position;
    vec3 direction;
    vec4 color;
    float intensity;
};

const int lightCount = 3;
uniform PointLightBlock {
     PointLight lights[lightCount];
} pointLights;

void main()
{
    vec2 texCoord = gl_FragCoord.xy / winSize;
    vec4 col = texture2D(color, texCoord);
    vec3 pos = texture2D(position, texCoord).xyz;
    vec3 norm = texture2D(normal, texCoord).xyz;

    vec4 lightColor;
    for (int i = 0; i < 3; i++) {
        vec3 s = normalize(pointLights.lights[i].position - pos);
        lightColor += pointLights.lights[i].color * (pointLights.lights[i].intensity * max(dot(s, norm), 0.0));
    }
    lightColor /= float(lightCount);
    fragColor = col * lightColor;
}
