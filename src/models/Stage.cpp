#include "Stage.h"
#include "Model.h"

Stage::Stage(glm::vec3 position = glm::vec3(0.0f)) : Project::Model::Model(position, glm::vec3(0.0f), glm::vec3(1.0f)) {
    centerPoint = 0.0f;
    radius = 0.0f;
    Steps = 22.0f;
    outerRad = 10.0f;
    innerRad = 3.0f;
    topHeight = 1.0f;
    bottomHeight = 0.1f;
    middlePoint = topHeight / bottomHeight;
}

Stage::Stage(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : Project::Model::Model(position, rotation, scale) {
    centerPoint = 0.0f;
    radius = 0.0f;
    Steps = 22.0f;
    outerRad = 10.0f;
    innerRad = 3.0f;
    topHeight = 1.0f;
    bottomHeight = 0.1f;
    middlePoint = topHeight / bottomHeight;
}


glm::mat4 partTranslationMatrix(GLfloat posX, GLfloat posY, GLfloat posZ) {
    return glm::translate(glm::mat4(1.0f), glm::vec3(posX, posY, posZ));
}

glm::mat4 partScalingMatrix(GLfloat widthScale, GLfloat heightScale, GLfloat depthScale) {
    return glm::scale(glm::mat4(1.0f), glm::vec3(widthScale, heightScale, depthScale));
}

void Stage::drawModel(Project::DrawContext context, glm::mat4 transformations) {
    GLfloat defaultSize = 0.125f;
    int shader = context.getShaderProgram();

    GLuint worldMatrixLocation = glGetUniformLocation(shader, "worldMatrix");
    glm::mat4 worldMatrix;

    glActiveTexture(GL_TEXTURE0);
    GLuint textureLocation = glGetUniformLocation(shader, "textureSampler");
    glBindTexture(GL_TEXTURE_2D, getTextureID());
    glUniform1i(textureLocation, 0);

    glm::mat4 scalingMatrix = scale(glm::mat4(1.0f), glm::vec3(1.2f, 0.9f, 1.2f));
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 translationMatrix = translate(glm::mat4(1.0f), glm::vec3(-0.25f, 0.0f, 0.0f));

    worldMatrix = scalingMatrix * transformations;
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawElements(GL_TRIANGLES, 400, GL_UNSIGNED_INT, 0);
}


/**
    * Returns the vertex buffer object for this model.
    **/
int Stage::generateVertexBufferObject() {
    // A vertex is a point on a polygon, it contains positions and other data (eg: colors)
    Project::TexturedVertex vertexArray[] = {
        // Front face


        //glm::vec3(0.0f, 1.0f, -5.0f),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(0.0f, 1.0f, -5.0f),  // bottom right color (green)
        Project::TexturedVertex(glm::vec3(0.0f, 1.0f, -5.0f),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(0.0f, 1.0f, -5.0f),glm::vec2(3.0f,  1.0f)),
        //glm::vec3(generatePointX(1), 1.0f, generatePointZ(1)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(1), 1.0f, generatePointZ(1)),  // point 1
        Project::TexturedVertex(glm::vec3(generatePointX(1), 1.0f, generatePointZ(1)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(1), 1.0f, generatePointZ(1)),glm::vec2(27.0f / 10.0f,  1.0f)),
        //glm::vec3(generatePointX(2), 1.0f, generatePointZ(2)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(2), 1.0f, generatePointZ(2)),  // point 2
        Project::TexturedVertex(glm::vec3(generatePointX(2), 1.0f, generatePointZ(2)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(2), 1.0f, generatePointZ(2)),glm::vec2(24.0f / 10.0f,  1.0f)),
        //glm::vec3(generatePointX(3), 1.0f, generatePointZ(3)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(3), 1.0f, generatePointZ(3)),  // point 3
        Project::TexturedVertex(glm::vec3(generatePointX(3), 1.0f, generatePointZ(3)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(3), 1.0f, generatePointZ(3)),glm::vec2(21.0f / 10.0f,  1.0f)),
        //glm::vec3(generatePointX(4), 1.0f, generatePointZ(4)), glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(4), 1.0f, generatePointZ(4)),  // point 4
        Project::TexturedVertex(glm::vec3(generatePointX(4), 1.0f, generatePointZ(4)), glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(4), 1.0f, generatePointZ(4)),glm::vec2(18.0f / 10.0f,  1.0f)),
        //glm::vec3(generatePointX(5), 1.0f, generatePointZ(5)),glm::vec3(0.5f, 0.35f, 0.05f), getNormalVector(generatePointX(5), 1.0f, generatePointZ(5)),  // point 5
        Project::TexturedVertex(glm::vec3(generatePointX(5), 1.0f, generatePointZ(5)),glm::vec3(0.5f, 0.35f, 0.05f), getNormalVector(generatePointX(5), 1.0f, generatePointZ(5)),glm::vec2(15.0f / 10.0f,  1.0f)),
        //glm::vec3(generatePointX(6), 1.0f, generatePointZ(6)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(6), 1.0f, generatePointZ(6)),  // point 6
        Project::TexturedVertex(glm::vec3(generatePointX(6), 1.0f, generatePointZ(6)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(6), 1.0f, generatePointZ(6)),glm::vec2(12.0f / 10.0f,  1.0f)),
        //glm::vec3(generatePointX(7), 1.0f, generatePointZ(7)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(7), 1.0f, generatePointZ(7)),  // point 7
        Project::TexturedVertex(glm::vec3(generatePointX(7), 1.0f, generatePointZ(7)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(7), 1.0f, generatePointZ(7)),glm::vec2(9.0f / 10.0f,  1.0f)),
        //glm::vec3(generatePointX(8), 1.0f, generatePointZ(8)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(8), 1.0f, generatePointZ(8)),  // point 8
        Project::TexturedVertex(glm::vec3(generatePointX(8), 1.0f, generatePointZ(8)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(8), 1.0f, generatePointZ(8)),glm::vec2(6.0f / 10.0f,  1.0f)),
        //glm::vec3(generatePointX(9), 1.0f, generatePointZ(9)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(9), 1.0f, generatePointZ(9)),  // point 9
        Project::TexturedVertex(glm::vec3(generatePointX(9), 1.0f, generatePointZ(9)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(9), 1.0f, generatePointZ(9)),glm::vec2(3.0f / 10.0f,  1.0f)),
        //glm::vec3(0.0f, 1.0f, 5.0f),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(0.0f, 1.0f, 5.0f),  // point 10
        Project::TexturedVertex(glm::vec3(0.0f, 1.0f, 5.0f),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(0.0f, 1.0f, 5.0f),glm::vec2(0.0f,  1.0f)),



        //Top inner ring
        //glm::vec3(0.0f, 1.0f, -1.50f),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(0.0f, 1.0f, -1.50f),  // point 11
        Project::TexturedVertex(glm::vec3(0.0f, 1.0f, -1.50f),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(0.0f, 1.0f, -1.50f) ,glm::vec2(3.0f,  0.0f)),
        //glm::vec3(generatePointXSmall(1), 1.0f, generatePointZSmall(1)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(1), 1.0f, generatePointZSmall(1)),  // point 12
        Project::TexturedVertex(glm::vec3(generatePointXSmall(1), 1.0f, generatePointZSmall(1)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(1), 1.0f, generatePointZSmall(1)) ,glm::vec2(2.7f,  0.0f)),
        //glm::vec3(generatePointXSmall(2), 1.0f, generatePointZSmall(2)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(2), 1.0f, generatePointZSmall(2)),  // point 13
        Project::TexturedVertex(glm::vec3(generatePointXSmall(2), 1.0f, generatePointZSmall(2)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(2), 1.0f, generatePointZSmall(2)) ,glm::vec2(2.4f,  0.0f)),
        //glm::vec3(generatePointXSmall(3), 1.0f,generatePointZSmall(3)), glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(3), 1.0f, generatePointZSmall(3)),  // point 14
        Project::TexturedVertex(glm::vec3(generatePointXSmall(3), 1.0f, generatePointZSmall(3)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(3), 1.0f, generatePointZSmall(3)) ,glm::vec2(2.1f,  0.0f)),
        //glm::vec3(generatePointXSmall(4), 1.0f,generatePointZSmall(4)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(4), 1.0f, generatePointZSmall(4)),  // point 15
        Project::TexturedVertex(glm::vec3(generatePointXSmall(4), 1.0f, generatePointZSmall(4)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(4), 1.0f, generatePointZSmall(4)) ,glm::vec2(1.8f,  0.0f)),
        //glm::vec3(generatePointXSmall(5), 1.0f, generatePointZSmall(5)),glm::vec3(0.5f, 0.35f, 0.05f) ,getNormalVectorReverse(generatePointXSmall(5), 1.0f, generatePointZSmall(5)), // point 16
        Project::TexturedVertex(glm::vec3(generatePointXSmall(5), 1.0f, generatePointZSmall(5)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(5), 1.0f, generatePointZSmall(5)) ,glm::vec2(1.5f,  0.0f)),
        //glm::vec3(generatePointXSmall(6), 1.0f, generatePointZSmall(6)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(6), 1.0f, generatePointZSmall(6)),  // point 17
        Project::TexturedVertex(glm::vec3(generatePointXSmall(6), 1.0f, generatePointZSmall(6)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(6), 1.0f, generatePointZSmall(6)) ,glm::vec2(1.2f,  0.0f)),
        //glm::vec3(generatePointXSmall(7), 1.0f, generatePointZSmall(7)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(7), 1.0f, generatePointZSmall(7)),  // point 18
        Project::TexturedVertex(glm::vec3(generatePointXSmall(7), 1.0f, generatePointZSmall(7)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(7), 1.0f, generatePointZSmall(7)) ,glm::vec2(0.9f,  0.0f)),
        //glm::vec3(generatePointXSmall(8), 1.0f, generatePointZSmall(8)), glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(8),1.0f, generatePointZSmall(8)),  // point 19
        Project::TexturedVertex(glm::vec3(generatePointXSmall(8), 1.0f, generatePointZSmall(8)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(8), 1.0f, generatePointZSmall(8)) ,glm::vec2(0.6f,  0.0f)),
        //glm::vec3(generatePointXSmall(9), 1.0f, generatePointZSmall(9)),glm::vec3(0.5f, 0.35f, 0.05f), getNormalVectorReverse(generatePointXSmall(9),1.0f, generatePointZSmall(9)),  // point 20
        Project::TexturedVertex(glm::vec3(generatePointXSmall(9), 1.0f, generatePointZSmall(9)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(9), 1.0f, generatePointZSmall(9)) ,glm::vec2(0.3f,  0.0f)),
        //glm::vec3(0.0f, 1.0f, 1.50f), glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(0.0f,1.0f, 1.5f), // point 21
        Project::TexturedVertex(glm::vec3(0.0f, 1.0f, 1.50f), glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(0.0f,1.0f, 1.5f),glm::vec2(0.0f, 0.0f)),


        //Bottom  outer circle

        //glm::vec3(0.0f, 0.0f, -1.0f * (outerRad / 2)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(0.0f, 0.0f, -1.0f * (outerRad / 2)),  // point 22
        Project::TexturedVertex(glm::vec3(0.0f, 0.0f, -1.0f * (outerRad / 2)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(0.0f, 0.0f, -1.0f * (outerRad / 2)),glm::vec2(3.0f,  0.0f)),
        //glm::vec3(generatePointX(1), 0.0f, generatePointZ(1)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(1), 0.0f, generatePointZ(1)),  // point 23
        Project::TexturedVertex(glm::vec3(generatePointX(1), 0.0f, generatePointZ(1)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(1), 0.0f, generatePointZ(1)),glm::vec2(2.7f,  0.0f)),
        //glm::vec3(generatePointX(2), 0.0f, generatePointZ(2)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(2), 0.0f, generatePointZ(2)),  // point 24
        Project::TexturedVertex(glm::vec3(generatePointX(2), 0.0f, generatePointZ(2)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(2), 0.0f, generatePointZ(2)),glm::vec2(2.4f,  0.0f)),
        //glm::vec3(generatePointX(3), 0.0f, generatePointZ(3)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(3), 0.0f, generatePointZ(3)),  // point 25
        Project::TexturedVertex(glm::vec3(generatePointX(3), 0.0f, generatePointZ(3)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(3), 0.0f, generatePointZ(3)),glm::vec2(2.1f,  0.0f)),
        //glm::vec3(generatePointX(4), 0.0f, generatePointZ(4)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(4), 0.0f, generatePointZ(4)),  // point 26
        Project::TexturedVertex(glm::vec3(generatePointX(4), 0.0f, generatePointZ(4)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(4), 0.0f, generatePointZ(4)),glm::vec2(1.8f,  0.0f)),
        //glm::vec3(generatePointX(5), 0.0f, generatePointZ(5)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(5), 0.0f, generatePointZ(5)),  // point 27
        Project::TexturedVertex(glm::vec3(generatePointX(5), 0.0f, generatePointZ(5)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(5), 0.0f, generatePointZ(5)),glm::vec2(1.5f,  0.0f)),
        //glm::vec3(generatePointX(6), 0.0f, generatePointZ(6)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(6), 0.0f, generatePointZ(6)),  // point 28
        Project::TexturedVertex(glm::vec3(generatePointX(6), 0.0f, generatePointZ(6)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(6), 0.0f, generatePointZ(6)),glm::vec2(1.2f,  0.0f)),
        //glm::vec3(generatePointX(7), 0.0f, generatePointZ(7)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(7), 0.0f, generatePointZ(7)),  // point 29
        Project::TexturedVertex(glm::vec3(generatePointX(7), 0.0f, generatePointZ(7)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(7), 0.0f, generatePointZ(7)),glm::vec2(0.9f,  0.0f)),
        //glm::vec3(generatePointX(8), 0.0f, generatePointZ(8)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(8), 0.0f, generatePointZ(8)),// point 30
        Project::TexturedVertex(glm::vec3(generatePointX(8), 0.0f, generatePointZ(8)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(8), 0.0f, generatePointZ(8)),glm::vec2(0.6f,  0.0f)),
        //glm::vec3(generatePointX(9), 0.0f, generatePointZ(9)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(9), 0.0f, generatePointZ(9)),  // point 31
        Project::TexturedVertex(glm::vec3(generatePointX(9), 0.0f, generatePointZ(9)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(generatePointX(9), 0.0f, generatePointZ(9)),glm::vec2(0.3f,  0.0f)),
        //glm::vec3(0.0f, 0.0f, 1.0f * (outerRad / 2)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(0.0f, 0.0f, 1.0f * (outerRad / 2)),  // point 32
        Project::TexturedVertex(glm::vec3(0.0f, 0.0f, 1.0f * (outerRad / 2)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVector(0.0f, 0.0f, 1.0f * (outerRad / 2)),glm::vec2(0.0f,  0.0f)),


        //bottom inner circle

        //glm::vec3(0.0f, 0.0f, -1.0f*(innerRad/2)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(0.0f, 0.0f, -1.0f * (outerRad / 2)),  // point 33
        Project::TexturedVertex(glm::vec3(0.0f, 0.0f, -1.0f * (innerRad / 2)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(0.0f, 0.0f, -1.0f * (outerRad / 2)),glm::vec2(3.0f,  1.0f)),
        //glm::vec3(generatePointXSmall(1), 0.0f, generatePointZSmall(1)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(1), 0.0f, generatePointZSmall(1)),  // point 34
        Project::TexturedVertex(glm::vec3(generatePointXSmall(1), 0.0f, generatePointZSmall(1)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(1), 0.0f, generatePointZSmall(1)),glm::vec2(2.7f,  1.0f)),
        //glm::vec3(generatePointXSmall(2), 0.0f, generatePointZSmall(2)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(2), 0.0f, generatePointZSmall(2)),  // point 35
        Project::TexturedVertex(glm::vec3(generatePointXSmall(2), 0.0f, generatePointZSmall(2)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(2), 0.0f, generatePointZSmall(2)),glm::vec2(2.4f,  1.0f)),
        //glm::vec3(generatePointXSmall(3), 0.0f,generatePointZSmall(3)),glm::vec3(0.5f, 0.35f, 0.05f), getNormalVectorReverse(generatePointXSmall(3), 0.0f, generatePointZSmall(3)),  // point 36
        Project::TexturedVertex(glm::vec3(generatePointXSmall(3), 0.0f, generatePointZSmall(3)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(3), 0.0f, generatePointZSmall(3)),glm::vec2(2.1f,  1.0f)),
        //glm::vec3(generatePointXSmall(4), 0.0f,generatePointZSmall(4)),glm::vec3(0.5f, 0.35f, 0.05f), getNormalVectorReverse(generatePointXSmall(4), 0.0f, generatePointZSmall(4)),  // point 37
        Project::TexturedVertex(glm::vec3(generatePointXSmall(4), 0.0f, generatePointZSmall(4)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(4), 0.0f, generatePointZSmall(4)),glm::vec2(1.8f,  1.0f)),
        //glm::vec3(generatePointXSmall(5), 0.0f, generatePointZSmall(5)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(5), 0.0f, generatePointZSmall(5)), // point 38
        Project::TexturedVertex(glm::vec3(generatePointXSmall(5), 0.0f, generatePointZSmall(5)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(5), 0.0f, generatePointZSmall(5)),glm::vec2(1.5f,  1.0f)),
        //glm::vec3(generatePointXSmall(6), 0.0f, generatePointZSmall(6)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(6), 0.0f, generatePointZSmall(6)),  // point 39
        Project::TexturedVertex(glm::vec3(generatePointXSmall(6), 0.0f, generatePointZSmall(6)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(6), 0.0f, generatePointZSmall(6)),glm::vec2(1.2f,  1.0f)),
        //glm::vec3(generatePointXSmall(7), 0.0f, generatePointZSmall(7)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(7), 0.0f, generatePointZSmall(7)),  // point 40
        Project::TexturedVertex(glm::vec3(generatePointXSmall(7), 0.0f, generatePointZSmall(7)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(7), 0.0f, generatePointZSmall(7)),glm::vec2(0.9f,  1.0f)),
        //glm::vec3(generatePointXSmall(8), 0.0f, generatePointZSmall(8)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(8), 0.0f, generatePointZSmall(8)),  // point 41
        Project::TexturedVertex(glm::vec3(generatePointXSmall(8), 0.0f, generatePointZSmall(8)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(8), 0.0f, generatePointZSmall(8)),glm::vec2(0.6f,  1.0f)),
        //glm::vec3(generatePointXSmall(9), 0.0f, generatePointZSmall(9)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(9), 0.0f, generatePointZSmall(9)),  // point 42
        Project::TexturedVertex(glm::vec3(generatePointXSmall(9), 0.0f, generatePointZSmall(9)),glm::vec3(0.5f, 0.35f, 0.05f),getNormalVectorReverse(generatePointXSmall(9), 0.0f, generatePointZSmall(9)),glm::vec2(0.3f,  1.0f)),
        //glm::vec3(0.0f, 0.0f, innerRad/2),glm::vec3(0.5f, 0.35f, 0.05f), getNormalVectorReverse(0.0f, 0.0f, innerRad / 2),  // point 43
        Project::TexturedVertex(glm::vec3(0.0f, 0.0f, innerRad / 2),glm::vec3(0.5f, 0.35f, 0.05f), getNormalVectorReverse(0.0f, 0.0f, innerRad / 2),glm::vec2(0.0f,  0.0f)),


        //glm::vec3(0.0f, 0.0f, innerRad / 2), glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(-1.0f, 0.0f, 0.0f),  // point 44 inner bottom 
        Project::TexturedVertex(glm::vec3(0.0f, 0.0f, innerRad / 2), glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec2(0.0f,  0.0f)),
        //glm::vec3(0.0f, 0.0f, 1.0f * (outerRad / 2)), glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(-1.0f, 0.0f, 0.0f),  // point 45 outer bottom
        Project::TexturedVertex(glm::vec3(0.0f, 0.0f, 1.0f * (outerRad / 2)), glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec2(1.0f,  0.0f)),
        //glm::vec3(0.0f, 1.0f, 1.50f), glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(-1.0f, 0.0f, 0.0f), // point 46 inner top
        Project::TexturedVertex(glm::vec3(0.0f, 1.0f, 1.50f), glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec2(1.0f,  1.0f)),
        //glm::vec3(0.0f, 1.0f, 5.0f), glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(-1.0f, 0.0f, 0.0f),  // point 47
        Project::TexturedVertex(glm::vec3(0.0f, 1.0f, 5.0f), glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec2(0.0f,  1.0f)),

        //manual top outer normal
        //glm::vec3(0.0f, 1.0f, -5.0f), glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),  //48
        Project::TexturedVertex(glm::vec3(0.0f, 1.0f, -5.0f), glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(3.0f,  1.0f)),

        //glm::vec3(generatePointX(1), 1.0f, generatePointZ(1)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),  // point 49
        Project::TexturedVertex(glm::vec3(generatePointX(1), 1.0f, generatePointZ(1)),glm::vec3(0.5f, 0.35f, 0.05f),glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(2.7f,  1.0f)),
        //glm::vec3(generatePointX(2), 1.0f, generatePointZ(2)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),  // point 50
        Project::TexturedVertex(glm::vec3(generatePointX(2), 1.0f, generatePointZ(2)),glm::vec3(0.5f, 0.35f, 0.05f),glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(2.4f,  1.0f)),
        //glm::vec3(generatePointX(3), 1.0f, generatePointZ(3)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),  // point 51
        Project::TexturedVertex(glm::vec3(generatePointX(3), 1.0f, generatePointZ(3)),glm::vec3(0.5f, 0.35f, 0.05f),glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(2.1f,  1.0f)),
        //glm::vec3(generatePointX(4), 1.0f, generatePointZ(4)), glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),  // point 52
        Project::TexturedVertex(glm::vec3(generatePointX(4), 1.0f, generatePointZ(4)),glm::vec3(0.5f, 0.35f, 0.05f),glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(1.8f,  1.0f)),
        //glm::vec3(generatePointX(5), 1.0f, generatePointZ(5)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),  // point 53
        Project::TexturedVertex(glm::vec3(generatePointX(5), 1.0f, generatePointZ(5)),glm::vec3(0.5f, 0.35f, 0.05f),glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(1.5f,  1.0f)),
        //glm::vec3(generatePointX(6), 1.0f, generatePointZ(6)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),  // point 54
        Project::TexturedVertex(glm::vec3(generatePointX(6), 1.0f, generatePointZ(6)),glm::vec3(0.5f, 0.35f, 0.05f),glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(1.2f,  1.0f)),
        //glm::vec3(generatePointX(7), 1.0f, generatePointZ(7)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),  // point 55
        Project::TexturedVertex(glm::vec3(generatePointX(7), 1.0f, generatePointZ(7)),glm::vec3(0.5f, 0.35f, 0.05f),glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.9f,  1.0f)),
        //glm::vec3(generatePointX(8), 1.0f, generatePointZ(8)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),  // point 56
        Project::TexturedVertex(glm::vec3(generatePointX(8), 1.0f, generatePointZ(8)),glm::vec3(0.5f, 0.35f, 0.05f),glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.6f,  1.0f)),
        //glm::vec3(generatePointX(9), 1.0f, generatePointZ(9)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),  // point 57
        Project::TexturedVertex(glm::vec3(generatePointX(9), 1.0f, generatePointZ(9)),glm::vec3(0.5f, 0.35f, 0.05f),glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.3f,  1.0f)),
        //glm::vec3(0.0f, 1.0f, 5.0f),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),  // point 58
        Project::TexturedVertex(glm::vec3(0.0f, 1.0f, 5.0f),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.0f,  1.0f)),

        //manuel inner top normal

        //glm::vec3(0.0f, 1.0f, -1.50f),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),  // point 59
        Project::TexturedVertex(glm::vec3(0.0f, 1.0f, -1.50f),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(3.0f,  0.0f)),

        //glm::vec3(generatePointXSmall(1), 1.0f, generatePointZSmall(1)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),  // point 60
        Project::TexturedVertex(glm::vec3(generatePointXSmall(1), 1.0f, generatePointZSmall(1)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(2.7f,  0.0f)),
        //glm::vec3(generatePointXSmall(2), 1.0f, generatePointZSmall(2)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),  // point 61
        Project::TexturedVertex(glm::vec3(generatePointXSmall(2), 1.0f, generatePointZSmall(2)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(2.4f,  0.0f)),
        //glm::vec3(generatePointXSmall(3), 1.0f, generatePointZSmall(3)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),  // point 62
        Project::TexturedVertex(glm::vec3(generatePointXSmall(3), 1.0f, generatePointZSmall(3)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(2.1f,  0.0f)),
        //glm::vec3(generatePointXSmall(4), 1.0f, generatePointZSmall(4)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),  // point 63
        Project::TexturedVertex(glm::vec3(generatePointXSmall(4), 1.0f, generatePointZSmall(4)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(1.8f,  0.0f)),
        //glm::vec3(generatePointXSmall(5), 1.0f, generatePointZSmall(5)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),  // point 64
        Project::TexturedVertex(glm::vec3(generatePointXSmall(5), 1.0f, generatePointZSmall(5)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(1.5f,  0.0f)),
        //glm::vec3(generatePointXSmall(6), 1.0f, generatePointZSmall(6)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),  // point 65
        Project::TexturedVertex(glm::vec3(generatePointXSmall(6), 1.0f, generatePointZSmall(6)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(1.2f,  0.0f)),
        //glm::vec3(generatePointXSmall(7), 1.0f, generatePointZSmall(7)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),  // point 66
        Project::TexturedVertex(glm::vec3(generatePointXSmall(7), 1.0f, generatePointZSmall(7)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.9f,  0.0f)),
        //glm::vec3(generatePointXSmall(8), 1.0f, generatePointZSmall(8)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),// point 67
        Project::TexturedVertex(glm::vec3(generatePointXSmall(8), 1.0f, generatePointZSmall(8)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.6f,  0.0f)),
        //glm::vec3(generatePointXSmall(9), 1.0f, generatePointZSmall(9)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),  // point 68
        Project::TexturedVertex(glm::vec3(generatePointXSmall(9), 1.0f, generatePointZSmall(9)),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.3f,  0.0f)),
        //glm::vec3(0.0f, 1.0f, 1.50f),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),  // point 69
        Project::TexturedVertex(glm::vec3(0.0f, 1.0f, 1.50f),glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.0f,  0.0f)),


        //glm::vec3(0.0f, 0.0f, -innerRad / 2), glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(-1.0f, 0.0f, 0.0f),  // point 70 inner bottom
        Project::TexturedVertex(glm::vec3(0.0f, 0.0f, -innerRad / 2), glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec2(1.0f,  0.0f)),
        //glm::vec3(0.0f, 0.0f, -1.0f * (outerRad / 2)), glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(-1.0f, 0.0f, 0.0f),  // point 71 outer bottom
        Project::TexturedVertex(glm::vec3(0.0f, 0.0f, -1.0f * (outerRad / 2)), glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec2(0.0f,  0.0f)),
        //glm::vec3(0.0f, 1.0f, -1.50f), glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(-1.0f, 0.0f, 0.0f), // point 72 inner top
        Project::TexturedVertex(glm::vec3(0.0f, 1.0f, -1.50f), glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec2(0.0f,  1.0f)),
        //glm::vec3(0.0f, 1.0f, -5.0f), glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(-1.0f, 0.0f, 0.0f),  // point 73
        Project::TexturedVertex(glm::vec3(0.0f, 1.0f, -5.0f), glm::vec3(0.5f, 0.35f, 0.05f), glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec2(1.0f,  1.0f)),








    };

    unsigned int indices[] = {
        //top  half circle
       // 11, 1, 0,
       // 11, 12, 1,

        //12, 2, 1,
        //12, 13, 2,

       // 13, 3, 2,
        //13, 14, 3,

        //14, 4, 3,
        //14, 15, 4,

       // 15, 5, 4,
        //15, 16, 5,

        //16,6,5,
        //16,17,6,

        //17,7,6,
        //17,18,7,

        //18,8,7,
        //18,19,8,

        //19,9,8,
        //19,20,9,

        //20,10,9,
        //20,21,10,


        //Bottom half circle

        33,22,23,
        33,23,34,

        34,23,24,
        34,24,35,

        35,24,25,
        35,25,36,

        36,25,26,
        36,26,37,

        37,26,27,
        37,27,38,

        38,27,28,
        38,28,39,

        39,28,29,
        39,29,40,

        40,29,30,
        40,30,41,

        41,30,31,
        41,31,42,

        42,31,32,
        42,32,43,


        //Side view
        33,12,11,
        33,34,12,

        34,13,12,
        34,35,13,

        35,14,13,
        35,36,14,

        36,15,14,
        36,37,15,

        37,16,15,
        37,38,16,

        38,17,16,
        38,39,17,

        39,18,17,
        39,40,18,

        40,19,18,
        40,41,19,

        41,20,19,
        41,42,20,

        42,21,20,
        42,43,21,

        //22,11,0,
        //33,11,22,

        23,22,0,
        23,0,1,

        24,23,1,
        24,1,2,

        25,24,2,
        25,2,3,

        26,25,3,
        26,3,4,

        27,26,4,
        27,4,5,

        28,27,5,
        28,5,6,

        29,28,6,
        29,6,7,

        30,29,7,
        30,7,8,

        31,30,8,
        31,8,9,

        32,31,9,
        32,9,10,

        //43,32,10,
        //43,10,21,

        /// manual normal fix
        44,45,47,
        44,47,46,

        59,49,48,
        59,60,49,

        60,50,49,
        60,61,50,

        61,51,50,
        61,62,51,

        62,52,51,
        62,63,52,

        63,53,52,
        63,64,53,

        64,54,53,
        64,65,54,

        65,55,54,
        65,66,55,

        66,56,55,
        66,67,56,

        67,57,56,
        67,68,57,

        68,58,57,
        68,69,58,

        71,70,72,
        71,72,73




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


glm::vec3 Stage::getNormalVector(float x, float y, float z) {
    glm::vec3 point = glm::vec3{ x,y,z };
    glm::vec3 middle = glm::vec3{ 0, middlePoint, 0 };
    glm::vec3 subsractedVector = point - middle;
    subsractedVector = subsractedVector * 1.0f;

    return  glm::normalize(subsractedVector);

}
glm::vec3 Stage::getNormalVectorReverse(float x, float y, float z) {
    glm::vec3 point = glm::vec3{ -1.0 * x,y,-1.0 * z };
    glm::vec3 middle = glm::vec3{ 0,middlePoint,0 };
    glm::vec3 subsractedVector = point - middle;
    subsractedVector = subsractedVector * 1.0f;

    return  glm::normalize(subsractedVector);
}

float Stage::generatePointX(int i) {
    float angle = 3.1415926 * 2.0f / Steps;
    float xpos = 0.0f, ypos = 0.0f, radius = outerRad / 2;
    float newx = radius * sin(angle * i);
    return newx;
}

float Stage::generatePointZ(int i) {
    float angle = 3.1415926 * 2.0f / Steps;
    float xpos = 0.0f, ypos = 0.0f, radius = outerRad / 2;
    float newY = -radius * cos(angle * i);
    return newY;
}

float Stage::generatePointZSmall(int i) {
    float angle = 3.1415926 * 2.0f / Steps;
    float xpos = 0.0f, ypos = 0.0f, radius = innerRad / 2;
    float newY = -radius * cos(angle * i);
    return newY;
}

float Stage::generatePointXSmall(int i) {
    float angle = 3.1415926 * 2.0f / Steps;
    float xpos = 0.0f, ypos = 0.0f, radius = innerRad / 2;
    float newx = radius * sin(angle * i);
    return newx;
}