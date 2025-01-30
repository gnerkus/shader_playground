## Color [graphics programming lecture]
### sRGB Color Space
- current standard for monitors, websites, images without an explicit color profile
- RGB values in the range 0.0 - 1.0
# Basic Lighting [learnopengl lighting]
- lighting in OpenGL is based on approximations of reality using simplified models. These lighting models are based on the physics of light as we understand it.
- The Phong lighting model consists of three parts: ambient, diffuse and specular lighting

## Ambient lighting [learnopengl lighting]
- objects are almost never completely dark
- ambient lighting is a constant that always gives the object some color to simulate that soft light source
- also know as global illumination
- can be represented by a simple algorithm:
```glsl
void main()
{
    vec4 texelColor = texture(texture0, fragTexCoord);
    
    float ambientStrength = 0.1;
    vec4 ambient = ambientStrength * lightColor;

    vec4 FragColor = ambient * texelColor;
    // when w is 1.0, the other co-ordinates are normalized
    // w is a scaling factor
    FragColor.w = 1.0;
} 
```

## Diffuse lighting [learnopengl lighting]
- simulates the directional impact a light object has on an object
- gives the object more brightness the closer its fragments are aligned to the light rays from a light source.
- if the light ray is perpendicular to the object's surface, the light has the greatest impact
- to measure the angle between the light ray and the fragment of the object it's hitting, we use a normal vector. The normal vector is a vector perpendicular to the fragment's surface.
- use dot product to get the angle between the light ray and the normal vector. the closer the dot product is to 1, the lower the angle between the two vectors, the lesser the effect of the light
### normal vector
- In the vertex shader from Raylib, this is represented by the value `vertexNormal`
```glsl
// Input vertex attributes
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec4 vertexColor;
```
- We send both the vertex normal and vertex position to the fragment shader:
```glsl
void main()
{
    // Send vertex attributes to fragment shader
    // 
    fragNormal = normalize(vec3(matNormal*vec4(vertexNormal, 1.0)));

    // Calculate final vertex position
    // mvp is the model-view-projection matrix
    gl_Position = mvp*vec4(vertexPosition, 1.0);
}
```
- To calculate the diffuse color, we need the light's position vector and the fragment's position vector.
- The light's position vector can be declared as a uniform:
```glsl
struct Light {
    // other light properties
    vec3 position;
};

// Input lighting values
uniform Light lights[MAX_LIGHTS];
```
- The uniform's value is set from the source code
```c
// Send to shader light position values
float position[3] = { light.position.x, light.position.y, light.position.z };
SetShaderValue(shader, light.positionLoc, position, SHADER_UNIFORM_VEC3);
```
- We need the fragment's position to be in world space. To accomplish this, we multiply the vertex position attribute with the model matrix (not the view and projection matrix) to transform it to world space co-ordinates.
```glsl
void main()
{
    // Send vertex attributes to fragment shader
    // matModel is the model's matrix
    fragPosition = vec3(matModel*vec4(vertexPosition, 1.0));
    
    fragNormal = normalize(vec3(matNormal*vec4(vertexNormal, 1.0)));

    // Calculate final vertex position
    gl_Position = mvp*vec4(vertexPosition, 1.0);
}
```
- We obtain the fragment's position in the fragment shader:
```glsl
in vec3 fragPosition;
```
- First, we need to calculate the direction vector between the light source and the fragment's position
```glsl
vec3 normal = normalize(fragNormal);
vec3 lightDir = normalize(light.position - fragPosition);
```
- In lighting calculations, make sure to always normalize the relevant vectors to ensure they're actual unit vectors since we only care about the direction and not the magnitude.
- Second, we calculate the diffuse impact of the light on the current fragment by taking the dot product between the normal and light direction vectors
```glsl
float NdotL = max(dot(normal, lightDir), 0.0);
vec3 diffuse = light.color.rgb * NdotL;
```
- Third, we can now combine both the ambient and diffuse
```glsl
vec4 texelColor = texture(texture0, fragTexCoord);
vec4 FragColor = (ambient + diffuse) * texelColor;
FragColor.w = 1.0;
```

## Specular lighting [learnopengl lighting]
- simulates the bright spot of a light that appears on shiny objects. Specular highlights are more inclined to the color of the light than the color of the object.

# Lighting shader explanation
## `viewPos`
The code below stores the location of the `viewPos` uniform from the fragment shader
**main.c**
```c
shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
```
**lighting.frag**
```glsl
uniform vec3 viewPos;
```
The `viewPos` is updated as the camera moves:
**main.c**
```c
float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
```

## `matModel`
`matModel` from the vertex, is automatically assigned on shader loading
**lighting.vert**
```glsl
uniform mat4 matModel;
```

## `ambient`
`ambient` from the fragment shader is accessed in main
**main.c**
```c
int ambientLoc = GetShaderLocation(shader, "ambient");
```
**lighting.frag**
```glsl
uniform vec4 ambient;
```
The ambient light is set to gray:
**main.c**
```c
SetShaderValue(shader, ambientLoc, (float[4]){ 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);
```

## Lights
The shader location for each light's properties are accessed using a template string:
**rlights.h**
```c
light.enabled = true;
light.type = type;
light.position = position;
light.target = target;
light.color = color;

// NOTE: Lighting shader naming must be the provided ones
// the location translates to GetShaderLocation(shader, lights[0].enabled)
light.enabledLoc = GetShaderLocation(shader, TextFormat("lights[%i].enabled", lightsCount));
light.typeLoc = GetShaderLocation(shader, TextFormat("lights[%i].type", lightsCount));
light.positionLoc = GetShaderLocation(shader, TextFormat("lights[%i].position", lightsCount));
light.targetLoc = GetShaderLocation(shader, TextFormat("lights[%i].target", lightsCount));
light.colorLoc = GetShaderLocation(shader, TextFormat("lights[%i].color", lightsCount));
```
Lights are updated by setting the values in the shader location:
**rlights.h**
```c
// light is passed in by reference (pointer)
void UpdateLightValues(Shader shader, Light light) {
    SetShaderValue(shader, light.enabledLoc, &light.enabled, SHADER_UNIFORM_INT);
}
```
The lights are defined in the shader as an array:
**lighting.frag**
```glsl
uniform Light lights[MAX_LIGHTS];
```
The lights in the shaders are applied via a loop:
**lighting.frag**
```c
for (int i = 0; i < MAX_LIGHTS; i++)
{
    if (lights[i].enabled == 1)
    {
        vec3 light = vec3(0.0);

        if (lights[i].type == LIGHT_DIRECTIONAL)
        {
            light = -normalize(lights[i].target - lights[i].position);
        }

        if (lights[i].type == LIGHT_POINT)
        {
            light = normalize(lights[i].position - fragPosition);
        }

        float NdotL = max(dot(normal, light), 0.0);
        lightDot += lights[i].color.rgb*NdotL;

        float specCo = 0.0;
        if (NdotL > 0.0) specCo = pow(max(0.0, dot(viewD, reflect(-(light), normal))), 16.0); // 16 refers to shine
        specular += specCo;
    }
}
```

# Raylib Shader values
Some uniforms are already set in shaders when they're loaded. See [`rlgl.h`](https://github.com/raysan5/raylib/blob/2492dd3d0a69efe77185daf708f9f8aa458113a8/src/rlgl.h#L78)
```c
When loading a shader, the following vertex attributes and uniform
*       location names are tried to be set automatically:
*
*       #define RL_DEFAULT_SHADER_ATTRIB_NAME_POSITION     "vertexPosition"    // Bound by default to shader location: RL_DEFAULT_SHADER_ATTRIB_LOCATION_POSITION
*       #define RL_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD     "vertexTexCoord"    // Bound by default to shader location: RL_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD
*       #define RL_DEFAULT_SHADER_ATTRIB_NAME_NORMAL       "vertexNormal"      // Bound by default to shader location: RL_DEFAULT_SHADER_ATTRIB_LOCATION_NORMAL
*       #define RL_DEFAULT_SHADER_ATTRIB_NAME_COLOR        "vertexColor"       // Bound by default to shader location: RL_DEFAULT_SHADER_ATTRIB_LOCATION_COLOR
*       #define RL_DEFAULT_SHADER_ATTRIB_NAME_TANGENT      "vertexTangent"     // Bound by default to shader location: RL_DEFAULT_SHADER_ATTRIB_LOCATION_TANGENT
*       #define RL_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD2    "vertexTexCoord2"   // Bound by default to shader location: RL_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD2
*       #define RL_DEFAULT_SHADER_ATTRIB_NAME_BONEIDS      "vertexBoneIds"     // Bound by default to shader location: RL_DEFAULT_SHADER_ATTRIB_LOCATION_BONEIDS
*       #define RL_DEFAULT_SHADER_ATTRIB_NAME_BONEWEIGHTS  "vertexBoneWeights" // Bound by default to shader location: RL_DEFAULT_SHADER_ATTRIB_LOCATION_BONEWEIGHTS
*       #define RL_DEFAULT_SHADER_UNIFORM_NAME_MVP         "mvp"               // model-view-projection matrix
*       #define RL_DEFAULT_SHADER_UNIFORM_NAME_VIEW        "matView"           // view matrix
*       #define RL_DEFAULT_SHADER_UNIFORM_NAME_PROJECTION  "matProjection"     // projection matrix
*       #define RL_DEFAULT_SHADER_UNIFORM_NAME_MODEL       "matModel"          // model matrix
*       #define RL_DEFAULT_SHADER_UNIFORM_NAME_NORMAL      "matNormal"         // normal matrix (transpose(inverse(matModelView)))
*       #define RL_DEFAULT_SHADER_UNIFORM_NAME_COLOR       "colDiffuse"        // color diffuse (base tint color, multiplied by texture color)
*       #define RL_DEFAULT_SHADER_UNIFORM_NAME_BONE_MATRICES  "boneMatrices"   // bone matrices
*       #define RL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE0  "texture0"          // texture0 (texture slot active 0)
*       #define RL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE1  "texture1"          // texture1 (texture slot active 1)
*       #define RL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE2  "texture2"          // texture2 (texture slot active 2)
```

# Coordinate systems
- OpenGL expects all the vertices, to be in normalized device coordinates. That is, the x,y,z of each vertex should be between 1.0 and -1.0
- To transform coordinates from one space to the next we'll use several transformation matrices 
  of which the most important are the model, view and projection matrix.
- Our vertex coordinates first start in local space as local coordinates and are then further processed to world coordinates, view coordinates, clip coordinates and eventually end up as screen coordinates.
    1. local space is transformed by the model matrix to world space
    2. world space is transformed by the view matrix to view space
    3. view space is transformed by the projection matrix to clip space
    4. clip space is transformed to screen space via viewport transform

## local space (object space)
- co-ordinates of the object relative to its local origin
- 

## world space
- co-ordinates relative to some global origin, together with manay other objects

## view space (eye space)
- co-ordinates as seen from the camera or viewer's point of view

## clip space
- processed to the 1.0 and -1.0 range
- determine which vertices end up on the screen
- will add perspective if using perspective projection

## screen space
- co-ordinates from -1.0 and 1.0 from the clip space are transformed to the co-ordinate range 
  defined by `glViewport`.
- co-ordinates from here are sent to the rasterizer to turn them into fragments.

### Explanation of the 'w' co-ordinate
- "The w component is a factor that divides the other vector components. When w is 1, the homogenous vector coordinates are normalized" [note](https://stackoverflow.com/a/2423060)
- 'I finally understood it when I read in the Red Book that "homogeneous vertex (x, y, z, w)T corresponds to the three-dimensional point (x/w, y/w, z/w)T" and that "the sequence of points (1, 2, 0, 1), (1, 2, 0, 0.01), and (1, 2.0, 0.0, 0.0001), corresponds to the euclidean points (1, 2), (100, 200), and (10000, 20000)"' [note](https://stackoverflow.com/questions/2422750/in-opengl-vertex-shaders-what-is-w-and-why-do-i-divide-by-it#comment60741262_2423060)




[graphics programming lecture]: https://www.mathematik.uni-marburg.de/~thormae/lectures/graphics1/graphics_10_1_eng_web.html#1
[learnopengl lighting]: https://learnopengl.com/Lighting/Basic-Lighting
[learnopengl coordinates]: https://learnopengl.com/Getting-started/Coordinate-Systems