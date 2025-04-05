#version 330 core

in vec2 vUV;
out vec4 FragColor;

uniform mat4 invView;
uniform mat4 invProjection;
uniform vec3 cameraPos;

const vec3 gridColor = vec3(0.0);
const vec3 axisColor = vec3(0.0);
const vec3 backgroundColor = vec3(0.85, 0.93, 1.0);

// Axis line thickness
const float axisThickness = 0.06;

// Grid levels: fine → coarse
const int NUM_LEVELS = 3;
const float spacings[NUM_LEVELS] = float[](0.25, 1.0, 5.0);
const float thicknesses[NUM_LEVELS] = float[](0.008, 0.02, 0.035);
const float switchHeights[NUM_LEVELS - 1] =
    float[](4.0, 12.0);  // transition midpoints

const float blendRange = 2.0;  // height range to blend between levels

// Optional distance-based fade
const float fadeStart = 100.0;
const float fadeEnd = 500.0;

float gridLine(float coord, float spacing, float thickness) {
    float d = abs(fract(coord / spacing) - 0.5);
    return smoothstep(thickness, 0.0, d);
}

float axisLine(float coord, float thickness) {
    return smoothstep(thickness, 0.0, abs(coord));
}

void main() {
    // Reconstruct ray direction
    vec4 ndc = vec4(vUV * 2.0 - 1.0, 1.0, 1.0);
    vec4 viewRay = invProjection * ndc;
    viewRay /= viewRay.w;
    vec3 rayDir = normalize((invView * vec4(viewRay.xyz, 0.0)).xyz);

    // Intersect with XZ plane (Y = 0)
    float t = -cameraPos.y / rayDir.y;
    if (t < 0.0) discard;

    vec3 hit = cameraPos + t * rayDir;
    float camHeight = abs(cameraPos.y);

    // Determine active LOD
    int baseLevel = 0;
    float blendFactor = 0.0;

    for (int i = 0; i < NUM_LEVELS - 1; ++i) {
        float center = switchHeights[i];
        if (camHeight < center - blendRange) {
            baseLevel = i;
            blendFactor = 0.0;
            break;
        } else if (camHeight < center + blendRange) {
            baseLevel = i;
            blendFactor =
                (camHeight - (center - blendRange)) / (2.0 * blendRange);
            break;
        }
    }

    if (camHeight > switchHeights[NUM_LEVELS - 2] + blendRange) {
        baseLevel = NUM_LEVELS - 1;
        blendFactor = 0.0;
    }

    // Sample base level
    float gx = gridLine(hit.x, spacings[baseLevel], thicknesses[baseLevel]);
    float gz = gridLine(hit.z, spacings[baseLevel], thicknesses[baseLevel]);
    float intensity = max(gx, gz) * (1.0 - blendFactor);

    // Sample upper level (if blending)
    if (blendFactor > 0.0 && baseLevel + 1 < NUM_LEVELS) {
        float gx2 = gridLine(hit.x, spacings[baseLevel + 1],
                             thicknesses[baseLevel + 1]);
        float gz2 = gridLine(hit.z, spacings[baseLevel + 1],
                             thicknesses[baseLevel + 1]);
        intensity += max(gx2, gz2) * blendFactor;
    }

    // Axis lines (always visible)
    float ax = axisLine(hit.x, axisThickness);
    float az = axisLine(hit.z, axisThickness);
    float axisIntensity = max(ax, az);

    // Optional fade with distance
    float distXZ = length(hit.xz - cameraPos.xz);
    float distFade =
        clamp((fadeEnd - distXZ) / (fadeEnd - fadeStart), 0.0, 1.0);

    // Final color
    vec3 color = backgroundColor;
    color = mix(color, gridColor, intensity * distFade);
    color = mix(color, axisColor, axisIntensity);

    FragColor = vec4(color, 1.0);
}
