#include "Model.h"
#include "TextureManager.h"
#include "Cubelet.h"
#include <map>
#include <iostream>

/**
 * Ideas:
 *  - Camera Rotation is around the rubiks cube
 *  - Limited translations (cube must be visible at all times)
 *  - Rotating rubiks cube is based on camera position and rubiks cube's orientation
 **/
Cubelet::Cubelet() : Project::Model::Model(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)) {
    _rubiksPosition = glm::vec3(-1.0f);
}

Cubelet::Cubelet(glm::vec3 position = glm::vec3(0.0f)) : Project::Model::Model(position, glm::vec3(0.0f), glm::vec3(1.0f)) {
    _rubiksPosition = glm::vec3(-1.0f);
}

void Cubelet::drawModel(Project::DrawContext context, glm::mat4 transformations) {
    int shader = context.getShaderProgram();
    GLuint worldMatrixLocation = glGetUniformLocation(shader, "worldMatrix");
    glm::mat4 worldMatrix;

    glActiveTexture(GL_TEXTURE0);
    GLuint textureLocation = glGetUniformLocation(shader, "textureSampler");

    /*glBindTexture(GL_TEXTURE_2D, getTexture());
    glUniform1i(textureLocation, 0);*/

    worldMatrix = transformations;
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);

    for (unsigned int i = 0; i < SIDES; i++) {
        glBindTexture(GL_TEXTURE_2D, getTextureAt((Side)i));
        glUniform1i(textureLocation, 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(sizeof(int) * SIDES * i));
    }
}

glm::vec3 Cubelet::getRubiksPosition() {
    return _rubiksPosition;
}

void Cubelet::setRubiksPosition(glm::vec3 rubiksPosition) {
    _rubiksPosition.x = rubiksPosition.x;
    _rubiksPosition.y = rubiksPosition.y;
    _rubiksPosition.z = rubiksPosition.z;
}

int Cubelet::getTextureAt(Side side) {
    return _textures[side];
}

void Cubelet::setTextureAt(Side side, int texture) {
    _textures[side] = texture;
}

int Cubelet::generateVertexBufferObject() {
    // A vertex is a point on a polygon, it contains positions and other data (eg: colors)
    Project::TexturedVertex vertexArray[] = {
        // Format: Position, Color, Normal, UV
        // Back side
        Project::TexturedVertex(glm::vec3(-0.5f, -0.5f, -0.5f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f,  0.0f, -1.0f),  glm::vec2(0.0f,  1.0f)), // Bottom left
        Project::TexturedVertex(glm::vec3(-0.5f, 0.5f, -0.5f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f,  0.0f)),// Bottom right
        Project::TexturedVertex(glm::vec3(0.5f,  0.5f, -0.5f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f,  0.0f)),// Top right
        Project::TexturedVertex(glm::vec3(0.5f,  -0.5f, -0.5f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f,  1.0f)),// Top left

        // Front side
        Project::TexturedVertex(glm::vec3(-0.5f, -0.5f,  0.5f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f,  1.0f)), // Bottom left
        Project::TexturedVertex(glm::vec3(0.5f, -0.5f,  0.5f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f,  1.0f)), // Bottom right
        Project::TexturedVertex(glm::vec3(0.5f,  0.5f,  0.5f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f,  0.0f)),// Top right
        Project::TexturedVertex(glm::vec3(-0.5f,  0.5f,  0.5f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f,  0.0f)),// Top left

        // Left
        Project::TexturedVertex(glm::vec3(-0.5f,  0.5f,  0.5f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f,  0.0f)),// Top front
        Project::TexturedVertex(glm::vec3(-0.5f,  0.5f, -0.5f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f,  0.0f)), // Top back
        Project::TexturedVertex(glm::vec3(-0.5f, -0.5f, -0.5f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f,  1.0f)),// Bottom back
        Project::TexturedVertex(glm::vec3(-0.5f, -0.5f,  0.5f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f,  1.0f)), // Bottom front

        // Right
        Project::TexturedVertex(glm::vec3(0.5f,  0.5f,  0.5f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f,  0.0f)),// Top front
        Project::TexturedVertex(glm::vec3(0.5f,  -0.5f, 0.5f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f,  1.0f)),// Top back
        Project::TexturedVertex(glm::vec3(0.5f, -0.5f, -0.5f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f,  1.0f)),// Bottom back
        Project::TexturedVertex(glm::vec3(0.5f, 0.5f,  -0.5f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f,  0.0f)),// Bottom front

        // Bottom
        Project::TexturedVertex(glm::vec3(-0.5f, -0.5f, -0.5f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f,  0.0f)),// Left back
        Project::TexturedVertex(glm::vec3(0.5f, -0.5f, -0.5f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f,  0.0f)),// Right back
        Project::TexturedVertex(glm::vec3(0.5f, -0.5f,  0.5f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f,  1.0f)),// Right front
        Project::TexturedVertex(glm::vec3(-0.5f, -0.5f,  0.5f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f,  1.0f)),// Left front

        // Top
        Project::TexturedVertex(glm::vec3(-0.5f,  0.5f, -0.5f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f,  0.0f)),// Left back
        Project::TexturedVertex(glm::vec3(-0.5f,  0.5f, 0.5f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f,  0.0f)),// Right back
        Project::TexturedVertex(glm::vec3(0.5f,  0.5f,  0.5f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f,  1.0f)),// Right front
        Project::TexturedVertex(glm::vec3(0.5f,  0.5f,  -0.5f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f,  1.0f))// Left front
    };

    unsigned int indices[] = {
        // Left
        0, 1, 2,
        0, 2, 3,
        // Right
        4, 5, 6,
        4, 6, 7,
        // Back
        10, 9, 8,
        11, 10, 8,
        // Front
        12, 13, 14,
        12, 14, 15,
        // Bottom
        16, 17, 18,
        16, 18, 19,
        // Top
        20, 22, 21,
        20, 23, 22
    };
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);


    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    GLuint elementBufferObject;
    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,                   // attribute 0 matches position in Vertex Shader
        4,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        sizeof(Project::TexturedVertex), // stride - each vertex contain 3 vec3 and 1 vec2 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,                            // attribute 1 matches color in Vertex Shader
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Project::TexturedVertex),
        (void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(
        2,                            // attribute 2 matches normal in Vertex Shader
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Project::TexturedVertex),
        (void*)(sizeof(glm::vec3) * 2)  // color is offseted 2 vec3 (comes after color)
    );
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(
        3,                            // attribute 3 matches UV coordinate in Vertex Shader
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Project::TexturedVertex),
        (void*)(sizeof(glm::vec3) * 3)  // color is offseted 2 vec3 (comes after color)
    );
    glEnableVertexAttribArray(3);

    return vertexArrayObject;
}