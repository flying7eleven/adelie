#version 450

// Updated bindings
layout(binding = 1) uniform sampler2D baseColorSampler;
layout(binding = 2) uniform sampler2D normalSampler;
layout(binding = 3) uniform sampler2D roughnessSampler;

// Updated inputs from vertex shader
layout(location = 0) in vec3 fragColor; // Unused, but passed
layout(location = 1) in vec3 fragNormal; // World space normal from VS
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec3 fragTangent; // World space tangent from VS

layout(location = 0) out vec4 outColor;

void main() {
    // Calculate TBN matrix
    vec3 N = normalize(fragNormal); // World space normal
    vec3 T = normalize(fragTangent); // World space tangent
    T = normalize(T - dot(T, N) * N); // Re-orthogonalize T with respect to N
    vec3 B = cross(N, T); // Calculate world space bitangent
    mat3 tbn = mat3(T, B, N); // Matrix to transform from tangent to world space

    // Sample normal map and transform to world space
    vec3 tangentNormal = texture(normalSampler, fragTexCoord).xyz * 2.0 - 1.0; // Map [0,1] -> [-1,1]
    vec3 worldNormal = normalize(tbn * tangentNormal); // Apply TBN matrix

    // Sample other maps
    vec4 baseColor = texture(baseColorSampler, fragTexCoord);
    float roughness = texture(roughnessSampler, fragTexCoord).r; // Assuming roughness in R channel

    // Simple diffuse lighting using the world normal from the normal map
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0)); // Example light direction
    float diff = max(dot(worldNormal, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0); // Light color = white
    
    // Combine (using original simple lighting for now, ignoring roughness)
    vec3 finalColor = (diffuse * 0.5 + 0.5) * baseColor.rgb;
    
    outColor = vec4(finalColor, 1.0);

    // TODO: Implement a PBR lighting model using roughness
} 