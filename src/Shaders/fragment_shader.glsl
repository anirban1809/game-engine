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

uniform vec3 viewPos;

void main() {
    // Normalize the normal and compute the light direction:
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    // Calculate the diffuse component:
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuseColor * lightColor;

    vec3 specularColor = vec3(1.0);

    // Specular component:
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
    vec3 specular = spec * specularColor * lightColor;

    // Sample the texture
    vec4 texColor = texture(texture1, TexCoord);

    // Multiply the texture color by the diffuse lighting:
    FragColor = vec4(diffuse + specular, 1.0);
    // FragColor = vec4(norm * 0.5 + 0.5, 1.0);

    // // Debug: Visualize normals as colors.
    // vec3 norm = normalize(Normal);
    // FragColor = vec4(norm * 0.5 + 0.5, 1.0);
}