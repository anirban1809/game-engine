#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform mat4 invView;
uniform mat4 invProjection;
uniform vec3 cameraPos;

const vec3 backgroundColor = vec3(0.702, 1.0, 0.761);  // Mint green
const vec3 gridColor = vec3(0.0);                      // Black
const vec3 axisColor = vec3(0.0);                      // Black

// Grid spacing (snap to powers of 2)
float getGridSpacing(float camHeight) {
    float exponent = floor(log2(max(camHeight, 0.001)));
    return pow(2.0, exponent);
}

// Grid line using fwidth AA
float gridLineAA(vec2 coord, float spacing) {
    vec2 dist = abs(mod(coord + spacing * 0.5, spacing) - spacing * 0.5);
    vec2 aaf = fwidth(coord);
    vec2 line = 1.0 - smoothstep(vec2(0.0), aaf, dist);
    return max(line.x, line.y);
}

// Axis line using fwidth AA
float axisLineAA(float coord) {
    float dist = abs(coord);
    float aa = fwidth(coord);
    return 1.0 - smoothstep(0.0, aa, dist);
}

// Dynamic distance fade range
float dynamicFadeEnd(float camHeight) {
    return clamp(camHeight * 10.0, 200.0,
                 1000.0);  // fadeEnd scales with height
}

float fadeByDistance(vec2 hitXZ, vec2 camXZ, float start, float end) {
    float d = length(hitXZ - camXZ);
    return clamp((end - d) / (end - start), 0.0, 1.0);
}

void main() {
    // Reconstruct ray
    vec4 ndc = vec4(vUV * 2.0 - 1.0, 1.0, 1.0);
    vec4 viewRay = invProjection * ndc;
    viewRay /= viewRay.w;
    vec3 rayDir = normalize((invView * vec4(viewRay.xyz, 0.0)).xyz);

    // Intersect with XZ plane
    float t = -cameraPos.y / rayDir.y;
    if (t < 0.0) discard;

    vec3 hit = cameraPos + t * rayDir;
    // float spacing = getGridSpacing(abs(cameraPos.y));

    float camHeight = abs(cameraPos.y);
    float spacing = getGridSpacing(camHeight);

    float grid = gridLineAA(hit.xz, spacing);
    float axis = max(axisLineAA(hit.x), axisLineAA(hit.z));

    // 📌 Apply dynamic distance fade
    float fadeEnd = dynamicFadeEnd(camHeight);
    float fadeStart = fadeEnd * 0.2;
    float fade = fadeByDistance(hit.xz, cameraPos.xz, fadeStart, fadeEnd);
    grid *= fade;

    // Final color mix
    vec3 color = mix(backgroundColor, gridColor, grid);
    color = mix(color, axisColor, axis);

    FragColor = vec4(color, 1.0);
}
