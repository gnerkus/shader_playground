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
- 

[graphics programming lecture]: https://www.mathematik.uni-marburg.de/~thormae/lectures/graphics1/graphics_10_1_eng_web.html#1
[learnopengl lighting]: https://learnopengl.com/Lighting/Basic-Lighting