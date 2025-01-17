#include "raylib.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

#define MAX_LIGHTS 4

//------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------
// light type
typedef enum {
    LIGHT_DIRECTIONAL = 0,
    LIGHT_POINT,
    LIGHT_SPOT
} LightType;

// Light data
typedef struct {
    int type;
    int enabled;
    Vector3 position;
    Vector3 target;
    float color[4];
    float intensity;

    // Shader light parameters locations
    int typeLoc;
    int enabledLoc;
    int positionLoc;
    int targetLoc;
    int colorLoc;
    int intensityLoc;
} Light;

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static int lightCount = 0;     // Current number of dynamic lights that have been created

//----------------------------------------------------------------------------------
// Module specific Functions Declaration
//----------------------------------------------------------------------------------
// Create a light and get shader locations
static Light CreateLight(int type, Vector3 position, Vector3 target, Color color, float intensity, Shader shader);

// Update light properties on shader
// NOTE: Light shader locations should be available
static void UpdateLight(Shader shader, Light light);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

    SetConfigFlags(FLAG_MSAA_4X_HINT);  // Enable Multi Sampling Anti Aliasing 4x (if available)
    InitWindow(screenWidth, screenHeight, "model load");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (Vector3){ 1.0f, 1.0f, 1.0f };    // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    Shader shader = LoadShader(TextFormat("resources/shaders/pbr.vert", GLSL_VERSION), TextFormat("resources/shaders/pbr.frag", GLSL_VERSION));
    shader.locs[SHADER_LOC_MAP_ALBEDO] = GetShaderLocation(shader, "albedoMap");
    // WARNING: Metalness, roughness, and ambient occlusion are all packed into a MRA texture
    // They are passed as to the SHADER_LOC_MAP_METALNESS location for convenience,
    // shader already takes care of it accordingly
    shader.locs[SHADER_LOC_MAP_METALNESS] = GetShaderLocation(shader, "mraMap");
    shader.locs[SHADER_LOC_MAP_NORMAL] = GetShaderLocation(shader, "normalMap");
    // WARNING: Similar to the MRA map, the emissive map packs different information
    // into a single texture: it stores height and emission data
    // It is binded to SHADER_LOC_MAP_EMISSION location an properly processed on shader
    shader.locs[SHADER_LOC_MAP_EMISSION] = GetShaderLocation(shader, "emissiveMap");
    shader.locs[SHADER_LOC_COLOR_DIFFUSE] = GetShaderLocation(shader, "albedoColor");

    // Setup additional required shader locations, including lights data
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    int lightCountLoc = GetShaderLocation(shader, "numOfLights");
    int maxLightCount = MAX_LIGHTS;
    SetShaderValue(shader, lightCountLoc, &maxLightCount, SHADER_UNIFORM_INT);

    // Setup ambient color and intensity parameters
    float ambientIntensity = 0.02f;
    Color ambientColor = (Color){ 26, 32, 135, 255 };
    Vector3 ambientColorNormalized = (Vector3){ ambientColor.r/255.0f, ambientColor.g/255.0f, ambientColor.b/255.0f };
    SetShaderValue(shader, GetShaderLocation(shader, "ambientColor"), &ambientColorNormalized, SHADER_UNIFORM_VEC3);
    SetShaderValue(shader, GetShaderLocation(shader, "ambient"), &ambientIntensity, SHADER_UNIFORM_FLOAT);

    // Get location for shader parameters that can be modified in real time
    int emissiveIntensityLoc = GetShaderLocation(shader, "emissivePower");
    int emissiveColorLoc = GetShaderLocation(shader, "emissiveColor");
    int textureTilingLoc = GetShaderLocation(shader, "tiling");

    // Raylib loads and unloads textures automatically for .gltf / .glb
    Model scene = LoadModel("resources/scene.gltf");

    // MATERIAL index + 1
    scene.materials[1].shader = shader;
//    scene.materials[2].shader = shader;
    scene.materials[3].shader = shader;
    scene.materials[4].shader = shader;
    scene.materials[5].shader = shader; // METAL
//    scene.materials[6].shader = shader;
//    scene.materials[7].shader = shader;

    // Setup materials[0].maps default parameters
    scene.materials[1].maps[MATERIAL_MAP_ALBEDO].color = WHITE;
    scene.materials[1].maps[MATERIAL_MAP_METALNESS].value = 0.0f;
    scene.materials[1].maps[MATERIAL_MAP_ROUGHNESS].value = 0.0f;
    // not sure about the occlusion and emission values
    scene.materials[1].maps[MATERIAL_MAP_OCCLUSION].value = 1.0f;
    scene.materials[1].maps[MATERIAL_MAP_EMISSION].color = (Color){ 255, 162, 0, 255 };

    // Setup materials[0].maps default parameters
//    scene.materials[5].maps[MATERIAL_MAP_ALBEDO].color = WHITE;
    scene.materials[5].maps[MATERIAL_MAP_METALNESS].value = 0.0f;
    scene.materials[5].maps[MATERIAL_MAP_ROUGHNESS].value = 0.0f;
    // not sure about the occlusion and emission values
    scene.materials[5].maps[MATERIAL_MAP_OCCLUSION].value = 1.0f;
    scene.materials[5].maps[MATERIAL_MAP_EMISSION].color = (Color){ 255, 162, 0, 255 };

    Vector3 position = { 0.0f, 0.0f, 0.0f };    // Set model position

    // Create some lights
    Light lights[MAX_LIGHTS] = { 0 };
    lights[0] = CreateLight(LIGHT_POINT, (Vector3){ -1.0f, 1.0f, -2.0f }, (Vector3){ 0.0f, 0.0f, 0.0f }, YELLOW, 4.0f, shader);
    lights[1] = CreateLight(LIGHT_POINT, (Vector3){ 2.0f, 1.0f, 1.0f }, (Vector3){ 0.0f, 0.0f, 0.0f }, GREEN, 3.3f, shader);
    lights[2] = CreateLight(LIGHT_POINT, (Vector3){ -2.0f, 1.0f, 1.0f }, (Vector3){ 0.0f, 0.0f, 0.0f }, RED, 8.3f, shader);
    lights[3] = CreateLight(LIGHT_POINT, (Vector3){ 1.0f, 1.0f, -2.0f }, (Vector3){ 0.0f, 0.0f, 0.0f }, BLUE, 2.0f, shader);

    // Setup material texture maps usage in shader
    // NOTE: By default, the texture maps are always used
    int usage = 1;
    SetShaderValue(shader, GetShaderLocation(shader, "useTexAlbedo"), &usage, SHADER_UNIFORM_INT);
    SetShaderValue(shader, GetShaderLocation(shader, "useTexNormal"), &usage, SHADER_UNIFORM_INT);
    SetShaderValue(shader, GetShaderLocation(shader, "useTexMRA"), &usage, SHADER_UNIFORM_INT);
    SetShaderValue(shader, GetShaderLocation(shader, "useTexEmissive"), &usage, SHADER_UNIFORM_INT);

    DisableCursor();                    // Limit cursor to relative movement inside the window
    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_FREE);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            BeginMode3D(camera);

                DrawModel(scene, position, 0.2f, WHITE);   // Draw 3d model with texture

                DrawGrid(10, 1.0f);     // Draw a grid

        // Draw spheres to show the lights positions
        for (int i = 0; i < MAX_LIGHTS; i++)
        {
            Color lightColor = (Color){ lights[i].color[0]*255, lights[i].color[1]*255, lights[i].color[2]*255, lights[i].color[3]*255 };

            if (lights[i].enabled) DrawSphereEx(lights[i].position, 0.2f, 8, 8, lightColor);
            else DrawSphereWires(lights[i].position, 0.2f, 8, 8, ColorAlpha(lightColor, 0.3f));
        }

            EndMode3D();

            DrawText("Cottage", screenWidth - 210, screenHeight - 20, 10, GRAY);

            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadModel(scene);         // Unload model

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Create light with provided data
// NOTE: It updated the global lightCount and it's limited to MAX_LIGHTS
static Light CreateLight(int type, Vector3 position, Vector3 target, Color color, float intensity, Shader shader)
{
    Light light = { 0 };

    if (lightCount < MAX_LIGHTS)
    {
        light.enabled = 1;
        light.type = type;
        light.position = position;
        light.target = target;
        light.color[0] = (float)color.r/255.0f;
        light.color[1] = (float)color.g/255.0f;
        light.color[2] = (float)color.b/255.0f;
        light.color[3] = (float)color.a/255.0f;
        light.intensity = intensity;

        // NOTE: Shader parameters names for lights must match the requested ones
        light.enabledLoc = GetShaderLocation(shader, TextFormat("lights[%i].enabled", lightCount));
        light.typeLoc = GetShaderLocation(shader, TextFormat("lights[%i].type", lightCount));
        light.positionLoc = GetShaderLocation(shader, TextFormat("lights[%i].position", lightCount));
        light.targetLoc = GetShaderLocation(shader, TextFormat("lights[%i].target", lightCount));
        light.colorLoc = GetShaderLocation(shader, TextFormat("lights[%i].color", lightCount));
        light.intensityLoc = GetShaderLocation(shader, TextFormat("lights[%i].intensity", lightCount));

        UpdateLight(shader, light);

        lightCount++;
    }

    return light;
}

// Send light properties to shader
// NOTE: Light shader locations should be available
static void UpdateLight(Shader shader, Light light)
{
    SetShaderValue(shader, light.enabledLoc, &light.enabled, SHADER_UNIFORM_INT);
    SetShaderValue(shader, light.typeLoc, &light.type, SHADER_UNIFORM_INT);

    // Send to shader light position values
    float position[3] = { light.position.x, light.position.y, light.position.z };
    SetShaderValue(shader, light.positionLoc, position, SHADER_UNIFORM_VEC3);

    // Send to shader light target position values
    float target[3] = { light.target.x, light.target.y, light.target.z };
    SetShaderValue(shader, light.targetLoc, target, SHADER_UNIFORM_VEC3);
    SetShaderValue(shader, light.colorLoc, light.color, SHADER_UNIFORM_VEC4);
    SetShaderValue(shader, light.intensityLoc, &light.intensity, SHADER_UNIFORM_FLOAT);
}