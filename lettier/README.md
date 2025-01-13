# Attribution
This work is based on ["Stylized Island cottage (mill)"](https://sketchfab.com/3d-models/stylized-island-cottage-mill-94f397a9598c4ed293a7934aa6cec892) by [SlagPerch 3D](https://sketchfab.com/slagperch3d) licensed under [CC-BY-4.0](http://creativecommons.org/licenses/by/4.0/)

# Simple 3D
Attempting to practice the techniques taught by [3D Game Shaders for Beginners](https://lettier.github.io/3d-game-shaders-for-beginners/index.html)

## Steps
### 1. Load scene
- Placed `.gltf`, `.bin` and _textures_ directory in the _resources_ directory
- Load the `.gltf`. This automatically loads the `.bin` which in turn, loads the associated textures and places them in the corresponding material slots
```shell
INFO: FILEIO: [resources/scene.gltf] File loaded successfully
INFO: MODEL: [resources/scene.gltf] Model basic data (glTF) loaded successfully
INFO:     > Meshes count: 54
INFO:     > Materials count: 16 (+1 default)
INFO: FILEIO: [resources/scene.bin] File loaded successfully
INFO: FILEIO: [./resources/textures/Barrel_baseColor.jpeg] File loaded successfully
INFO: IMAGE: Data loaded successfully (512x128 | R8G8B8 | 1 mipmaps)
INFO: TEXTURE: [ID 3] Texture loaded successfully (512x128 | R8G8B8 | 1 mipmaps)
INFO: FILEIO: [./resources/textures/Rocks_baseColor.jpeg] File loaded successfully
INFO: IMAGE: Data loaded successfully (512x512 | R8G8B8 | 1 mipmaps)
INFO: TEXTURE: [ID 4] Texture loaded successfully (512x512 | R8G8B8 | 1 mipmaps)
INFO: FILEIO: [./resources/textures/Wood_fence_baseColor.jpeg] File loaded successfully
INFO: IMAGE: Data loaded successfully (512x512 | R8G8B8 | 1 mipmaps)
INFO: TEXTURE: [ID 5] Texture loaded successfully (512x512 | R8G8B8 | 1 mipmaps)
INFO: FILEIO: [./resources/textures/Wood_molding_baseColor.jpeg] File loaded successfully
INFO: IMAGE: Data loaded successfully (512x512 | R8G8B8 | 1 mipmaps)
INFO: TEXTURE: [ID 6] Texture loaded successfully (512x512 | R8G8B8 | 1 mipmaps)
INFO: FILEIO: [./resources/textures/Metal_metallicRoughness.png] File loaded successfully
INFO: IMAGE: Data loaded successfully (1024x1024 | GRAYSCALE | 1 mipmaps)
INFO: TEXTURE: [ID 7] Texture loaded successfully (1024x1024 | GRAYSCALE | 1 mipmaps)
INFO: FILEIO: [./resources/textures/Tiles_baseColor.jpeg] File loaded successfully
INFO: IMAGE: Data loaded successfully (512x512 | R8G8B8 | 1 mipmaps)
INFO: TEXTURE: [ID 8] Texture loaded successfully (512x512 | R8G8B8 | 1 mipmaps)
INFO: FILEIO: [./resources/textures/Walls_baseColor.png] File loaded successfully
INFO: IMAGE: Data loaded successfully (1024x1024 | R8G8B8 | 1 mipmaps)
INFO: TEXTURE: [ID 9] Texture loaded successfully (1024x1024 | R8G8B8 | 1 mipmaps)
INFO: FILEIO: [./resources/textures/Walls_metallicRoughness.png] File loaded successfully
INFO: IMAGE: Data loaded successfully (1024x1024 | R8G8B8 | 1 mipmaps)
INFO: TEXTURE: [ID 10] Texture loaded successfully (1024x1024 | R8G8B8 | 1 mipmaps)
INFO: FILEIO: [./resources/textures/Walls_normal.png] File loaded successfully
INFO: IMAGE: Data loaded successfully (1024x1024 | R8G8B8 | 1 mipmaps)
INFO: TEXTURE: [ID 11] Texture loaded successfully (1024x1024 | R8G8B8 | 1 mipmaps)
INFO: FILEIO: [./resources/textures/Bricks_baseColor.jpeg] File loaded successfully
INFO: IMAGE: Data loaded successfully (1024x1024 | R8G8B8 | 1 mipmaps)
INFO: TEXTURE: [ID 12] Texture loaded successfully (1024x1024 | R8G8B8 | 1 mipmaps)
INFO: FILEIO: [./resources/textures/Island_baseColor.jpeg] File loaded successfully
INFO: IMAGE: Data loaded successfully (1024x1024 | R8G8B8 | 1 mipmaps)
INFO: TEXTURE: [ID 13] Texture loaded successfully (1024x1024 | R8G8B8 | 1 mipmaps)
INFO: FILEIO: [./resources/textures/House_baseColor.png] File loaded successfully
INFO: IMAGE: Data loaded successfully (1024x1024 | R8G8B8 | 1 mipmaps)
INFO: TEXTURE: [ID 14] Texture loaded successfully (1024x1024 | R8G8B8 | 1 mipmaps)
INFO: FILEIO: [./resources/textures/House_metallicRoughness.png] File loaded successfully
INFO: IMAGE: Data loaded successfully (1024x1024 | R8G8B8 | 1 mipmaps)
INFO: TEXTURE: [ID 15] Texture loaded successfully (1024x1024 | R8G8B8 | 1 mipmaps)
INFO: FILEIO: [./resources/textures/House_normal.png] File loaded successfully
INFO: IMAGE: Data loaded successfully (1024x1024 | R8G8B8 | 1 mipmaps)
INFO: TEXTURE: [ID 16] Texture loaded successfully (1024x1024 | R8G8B8 | 1 mipmaps)
INFO: FILEIO: [./resources/textures/Grass_baseColor.jpeg] File loaded successfully
INFO: IMAGE: Data loaded successfully (512x512 | R8G8B8 | 1 mipmaps)
INFO: TEXTURE: [ID 17] Texture loaded successfully (512x512 | R8G8B8 | 1 mipmaps)
```
- PBR shaders are missing

This work is based on ["Stylized Island cottage (mill)"](https://sketchfab.com/3d-models/stylized-island-cottage-mill-94f397a9598c4ed293a7934aa6cec892) by [SlagPerch 3D](https://sketchfab.com/slagperch3d) licensed under [CC-BY-4.0](http://creativecommons.org/licenses/by/4.0/)