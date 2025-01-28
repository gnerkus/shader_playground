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




[graphics programming lecture]: https://www.mathematik.uni-marburg.de/~thormae/lectures/graphics1/graphics_10_1_eng_web.html#1
[learnopengl lighting]: https://learnopengl.com/Lighting/Basic-Lighting