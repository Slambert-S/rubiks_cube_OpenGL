
#include <iostream>
#include "Floor.h""
#include "Model.h"
#include "TextureManager.h"



    Floor::Floor(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : Project::Model::Model(position, rotation, scale) {
        setVertexBufferObject(generateVertexBufferObject());
    }
    void Floor::drawModel(Project::DrawContext context, glm::mat4 transformations) {
        int shader = context.getShaderProgram();

        GLuint worldMatrixLocation = glGetUniformLocation(shader, "worldMatrix");

        glm::mat4 worldMatrix;

        glActiveTexture(GL_TEXTURE0);
        GLuint textureLocation = glGetUniformLocation(shader, "textureSampler");
        glBindTexture(GL_TEXTURE_2D, getTextureID());
        glUniform1i(textureLocation, 0);
      
        worldMatrix = glm::translate(transformations, glm::vec3(0.0f, -0.03f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(64.0f * 0.35f, 0.02f, 64.0f * 0.35f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }


    /**
     * Returns the vertex buffer object for this model.
     **/
    int Floor::generateVertexBufferObject() {


        // A vertex is a point on a polygon, it contains positions and other data (eg: colors)

        Project::TexturedVertex vertexArray[] = {
            // Format: Position, Color, Normal, UV
            // Back side
            Project::TexturedVertex(glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f,  0.0f, -1.0f),  glm::vec2(0.0f,  0.0f)), // Bottom left
            Project::TexturedVertex(glm::vec3(-1.0f, 1.0f, -1.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f,  0.0f)),// Bottom right
            Project::TexturedVertex(glm::vec3(1.0f,  1.0f, -1.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f,  1.0f)),// Top right
            Project::TexturedVertex(glm::vec3(1.0f,  -1.0f, -1.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f,  1.0f)),// Top left

            // Front side
            Project::TexturedVertex(glm::vec3(-1.0f, -1.0f,  1.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f,  0.0f)), // Bottom left
            Project::TexturedVertex(glm::vec3(1.0f, -1.0f,  1.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f,  0.0f)), // Bottom right
            Project::TexturedVertex(glm::vec3(1.0f,  1.0f,  1.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(1.0f,  1.0f)),// Top right
            Project::TexturedVertex(glm::vec3(-1.0f,  1.0f,  1.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f,  0.0f,  1.0f), glm::vec2(0.0f,  1.0f)),// Top left

            // Left
            Project::TexturedVertex(glm::vec3(-1.0f,  1.0f,  1.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f,  0.0f)),// Top front
            Project::TexturedVertex(glm::vec3(-1.0f,  1.0f, -1.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f,  0.0f)), // Top back
            Project::TexturedVertex(glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f,  1.0f)),// Bottom back
            Project::TexturedVertex(glm::vec3(-1.0f, -1.0f,  1.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f,  1.0f)), // Bottom front

            // Right
            Project::TexturedVertex(glm::vec3(1.0f,  1.0f,  1.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f,  0.0f)),// Top front
            Project::TexturedVertex(glm::vec3(1.0f,  -1.0f, 1.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f,  0.0f)),// Top back
            Project::TexturedVertex(glm::vec3(1.0f, -1.0f, -1.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f,  1.0f)),// Bottom back
            Project::TexturedVertex(glm::vec3(1.0f, 1.0f,  -1.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f,  1.0f)),// Bottom front

            // Bottom
            Project::TexturedVertex(glm::vec3(-1.0f, -1.0f, -1.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f,  0.0f)),// Left back
            Project::TexturedVertex(glm::vec3(1.0f, -1.0f, -1.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f,  0.0f)),// Right back
            Project::TexturedVertex(glm::vec3(1.0f, -1.0f,  1.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f,  1.0f)),// Right front
            Project::TexturedVertex(glm::vec3(-1.0f, -1.0f,  1.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f,  1.0f)),// Left front

            // Top
            Project::TexturedVertex(glm::vec3(-1.0f,  1.0f, -1.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f,  0.0f)),// Left back
            Project::TexturedVertex(glm::vec3(-1.0f,  1.0f, 1.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(16.0f,  0.0f)),// Right back
            Project::TexturedVertex(glm::vec3(1.0f,  1.0f,  1.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(16.0f,  16.0f)),// Right front
            Project::TexturedVertex(glm::vec3(1.0f,  1.0f,  -1.0f),glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f,  16.0f))// Left front
        };

        unsigned int indices[] = {
            // Back
            0, 1, 2,
            0, 2, 3,
            // Front
            4, 5, 6,
            4, 6, 7,
            // Left
            8, 9, 10,
            8, 10, 11,
            // Right
            12, 13, 14,
            12, 14, 15,
            // Bottom
            16, 17, 18,
            16, 18, 19,
            // Top
            20, 21, 22,
            20, 22, 23
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
