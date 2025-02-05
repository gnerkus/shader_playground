#version 330

// Input vertex attributes (from vertex shader)
// ======================================================================

// position vector for each fragment
in vec3 fragPosition;
// texture coordinate for each fragment
in vec2 fragTexCoord;
// rgba color for each fragment
in vec4 fragColor;
// normal vector for each fragment
in vec3 fragNormal;

// Input uniform values; these are automatically filled by Raylib
// =======================================================================
// Fragment input texture; required
uniform sampler2D texture0;
// Fragment input color diffuse; multiplied by texture colour
uniform vec4 colDiffuse;

// User-defined uniform values
// =======================================================================
// camera position vector
uniform vec3 viewPos;
// diffuse light position
uniform vec3 lightPos;

// Output fragment color
out vec4 finalColor;

void main() {
    // Texel color fetching from texture sampler
    vec4 texelColor = texture(texture0, fragTexCoord);

    vec3 lightDot = vec3(0.0);
    vec3 normal = normalize(fragNormal);
    vec3 viewD = normalize(viewPos - fragPosition);
    vec3 specular = vec3(0.0);

    vec4 tint = colDiffuse * fragColor;

    // POINT LIGHT
    vec3 lightDirection = normalize(lightPos - fragPosition);
}