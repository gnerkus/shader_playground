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
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = ambient * objectColor;
    FragColor = vec4(result, 1.0);
} 
```

## Diffuse lighting [learnopengl lighting]
- simulates the directional impact a light object has on an object


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




[graphics programming lecture]: https://www.mathematik.uni-marburg.de/~thormae/lectures/graphics1/graphics_10_1_eng_web.html#1
[learnopengl lighting]: https://learnopengl.com/Lighting/Basic-Lighting