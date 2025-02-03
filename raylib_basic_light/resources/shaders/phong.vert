#version 330

// Input vertex attributes
// ===================================================================================

// vertex position
in vec3 vertexPosition;
// vertex input attribute: texture coordinate
in vec2 vertexTexCoord;
// vertex normal
in vec3 vertexNormal;
// vertex color
in vec4 vertexColor;

// Input uniform values; These are automatically filled by Raylib
// ====================================================================================

// Model-view-projection matrix
uniform mat4 mvp;
// The model's position matrix
// A transformation matrix that translates, scales and/or rotates the model to place it
// in the world at a location/orientation they belong to.
uniform mat4 matModel;
// The model's normal matrix
uniform mat4 matNormal;

// Output vertex attributes (to fragment shader)
// ======================================================================================
out vec3 fragPosition;
out vec2 fragTexCoord;
out vec4 fragColor;
out vec3 fragNormal;

void main() {
    // Send vertex attributes to fragment shader
    /**
        1. Use vec4 to add the w dimension to each vertex' position
        2. multiply the result by the model's position matrix to transform the positions from local to world space
        3. convert to a vector 3 (remove the w dimension)
    */
    fragPosition = vec3(matModel*vec4(vertexPosition, 1.0));
    /**
        1. Copy over the texture co-ordinates from the vertex to fragment shader
    */
    fragTexCoord = vertexTexCoord;
    /**
        1. Copy over the vertex colors to the fragment shader
    */
    fragColor = vertexColor;
    /**
        1. Use vec4() to add the w dimension to each vertex' normal
        2. multiply the result by the model's normal matrix to transform the normals from local to world space
        3. Remove the w dimension with vec3()
        4. convert the normals to unit vectors with normalize()
    */
    fragNormal = normalize(vec3(matNormal*vec4(vertexNormal, 1.0)));

    // Calculate final vertex position
    /**
        1. Add the w dimension to each vertex position with vec4()
        2. Multiply the vertex positions by the model-view-projection matrix to perform the following transformations:
            2.1 local space -> world space (model matrix)
            2.2 world space -> view space (view matrix: each coordinate is seen from camera's point of view)
            2.3 view space -> clip space (projection matrix: determines which vertices end up on the screen; adds perspective
    */
    gl_Position = mvp*vec4(vertexPosition, 1.0);
}
