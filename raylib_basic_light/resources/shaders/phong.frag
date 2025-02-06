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

#define     MAX_LIGHTS              4
#define     LIGHT_DIRECTIONAL       0
#define     LIGHT_POINT             1

struct Light {
    int enabled;
    int type;
    vec3 position;
    vec3 target;
    vec4 color;
};

// Input lighting values
uniform Light lights[MAX_LIGHTS];
// ambient color
uniform vec4 ambient;

// Output fragment color
out vec4 finalColor;

void main() {
    // Texel color fetching from texture sampler
    vec4 texelColor = texture(texture0, fragTexCoord);

    // diffuse aspect init
    vec3 lightDot = vec3(0.0);
    // specular aspect init
    vec3 specular = vec3(0.0);
    // fragment normal is imported from the vertex shader
    vec3 normal = normalize(fragNormal);
    vec3 viewD = normalize(viewPos - fragPosition);

    vec4 tint = colDiffuse * fragColor;

    // POINT LIGHT
    /**
        Compute light's direction vector as a difference between the light's position vector and the frag's
        position vector.
    */
    vec3 lightDirection = normalize(lights[0].position - fragPosition);

    /**
        Calculate the angle between the light vector and the fragment normal
    */
    float NdotL = max(dot(normal, lightDirection), 0.0);
    /**
        DIFFUSE
        Calculate the color of the diffuse aspect of the surface
    */
    lightDot += lights[0].color.rgb * NdotL;
    /**
        SPECULAR
    */
}