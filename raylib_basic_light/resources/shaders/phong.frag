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
    /**
        Compute the camera's direction vector as the difference between the camera's position and the frag's
        position vector.

        This is the direction of the fragment from the point of view of the camera
    */
    vec3 viewD = normalize(viewPos - fragPosition);

    vec4 tint = colDiffuse * fragColor;

    // POINT LIGHT
    /**
        Compute light's direction vector as the difference between the light's position vector and the frag's
        position vector.

        This is the direction of the reflected light.
    */
    vec3 lightDirection = normalize(lights[0].position - fragPosition);

    /**
        Calculate the strength of the reflection of the light source off the fragment

        The dot product is the difference between the reflected light's vector and the fragment normal vector

        If both vectors point in the same direction, the dot product is at its maximum.
        If both vectors are perpendicular, the dot product is 0
        If both vectors point in different directions, the dot product is negative with max value

        ref: https://www.khanacademy.org/math/multivariable-calculus/thinking-about-multivariable-function/x786f2022:vectors-and-matrices/a/dot-products-mvc
    */
    float NdotL = max(dot(normal, lightDirection), 0.0);
    /**
        DIFFUSE
        Calculate the color of the diffuse aspect of the surface

        - we use the lights color for the diffuse
    */
    lightDot += lights[0].color.rgb * NdotL;

    float specCo = 0.0;
    /**
        SPECULAR
        Calculate the color of the specular aspect of the surface

        specCo = pow(
            max(
                0.0, 
                dot(
                    viewD, 
                    reflect(
                        -(lightDirection), 
                        normal
                    )
                )
            ), 
            16.0
        );

        1. First, get the resulting vector when an incident vector (I) of `-lightDirection`
            is reflected off a surface with normal (N) `normal`. This is calculated as:
            I - 2.0 * dot(N, I) * N
        2.  Get the difference between the camera's direction and the reflected light (via dot product)
            Ensure this difference is never below 0.
            This will determine how much of the specular is visible to the camera
        3. Magnify the effect by a shininess factor e.g 16

        - we also use the lights color for the specular
        - TODO: define a separate uniform to contain the specular color
        - 16 is the shininess
        - TODO: define a uniform to contain the specular shininess
    */
    if(NdotL > 0.0)
        specCo = pow(max(0.0, dot(viewD, reflect(-(lightDirection), normal))), 16.0);
    specular += specCo;

    /**
        1. Apply the tint color to the specular color
        2. multiply the texture colour by the diffuse and tinted specular colour
        3. Apply the tint color to the ambient light
        4. Multiply the texture colour by the ambient light colour
        5. Add the result to the final colour
    */
    finalColor = (texelColor * ((tint + vec4(specular, 1.0)) * vec4(lightDot, 1.0)));
    finalColor += texelColor * (ambient / 10.0) * tint;

    // Gamma correction
    finalColor = pow(finalColor, vec4(1.0 / 2.2));
}