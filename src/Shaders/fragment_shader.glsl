#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D texture1;  // The diffuse texture

// Light and material uniforms
uniform vec3 lightPos;      // Light position in world space
uniform vec3 lightColor;    // Light color (intensity)
uniform vec3 diffuseColor;  // Material diffuse color

void main() {
    // Normalize the normal and compute the light direction:
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    // Calculate the diffuse component:
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuseColor * lightColor;

    // Sample the texture
    vec4 texColor = texture(texture1, TexCoord);

    // Multiply the texture color by the diffuse lighting:
    FragColor = vec4(diffuse, 1.0);
    // FragColor = vec4(norm * 0.5 + 0.5, 1.0);

    // // Debug: Visualize normals as colors.
    // vec3 norm = normalize(Normal);
    // FragColor = vec4(norm * 0.5 + 0.5, 1.0);
}