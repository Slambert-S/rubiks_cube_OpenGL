#include "Model.h"

/**
* The Model class can be used for "parent" models as well as any hierarchically dependant "children" models.
* They have as an attribute a parent transformation matrix to be applied to each part of the model passed on by the parent
* or to be set to the Identity for a first generation model. The position, rotation, and scaling values can be calculated into a matrix to
* be applied to the model in question or passed to its children as a parentMatrix.
* 
 * Models each have their own shader program and buffer object.
 * While that is slower than placing them all together, it facilitates
 * drawing several objects that have no relation to each other.
 * 
 * Models have child models that will be drawn after the parent model
 * is drawn. Children will keep the transformations of the parent model.
 **/

Project::Model::Model(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
    _position = position;
    _rotation = rotation;
    _scale = scale;
    _shear = glm::mat4(1.0f);
    _children = std::vector<Model*>();
    _numChildren = 0;
    _textureID = 0;
    _type = 0;
}

void Project::Model::draw(DrawContext context, glm::mat4 transformations) {
    int shaderProgram = context.getShaderProgram();
    int vao = getVertexBufferObject();
    if (vao <= 0) {
        vao = generateVertexBufferObject();
    }
    if (vao > 0) {
        setVertexBufferObject(vao);
    }
    glBindVertexArray(vao);

    /*if (getTextureID() > 0) {
        GLuint textureLocation = glGetUniformLocation(shaderProgram, "textureSampler");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, getTextureID());
        glUniform1i(textureLocation, 0);
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);
    }*/

    glm::mat4 identity(1.0f);
    glm::mat4 translation = glm::translate(identity, getPosition());
    glm::mat4 scale = glm::scale(identity, getScale());
    glm::mat4 rotationX = glm::rotate(identity, getRotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotationY = glm::rotate(identity, getRotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotationZ = glm::rotate(identity, getRotation().z, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 shearing = getShearing();
    glm::mat4 matrix = transformations * translation * scale * rotationZ * rotationY * rotationX * shearing;
    drawModel(context, matrix);

    for (Project::Model* child : _children) {
        (*child).draw(context, matrix);
    }
}

void Project::Model::addChild(Project::Model* model) {
    _children.push_back(model);
    _numChildren++;
}

glm::vec3 Project::Model::getPosition() {
    return _position;
}

void Project::Model::setPosition(glm::vec3 position) {
    _position = position;
}

void Project::Model::setScaling(glm::vec3 scale) {
    _scale = scale;
}

glm::vec3 Project::Model::getScale() {
    return _scale;
}

void Project::Model::setRotation(glm::vec3 rotation) {
    _rotation = rotation;
}

glm::vec3 Project::Model::getRotation() {
    return _rotation;
}

void Project::Model::setShearing(glm::vec3 shear) {
    GLfloat m[16] = {
        1.0f, 0.0f, shear.z, 0.0f,
        shear.x, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    _shear = glm::make_mat4(m);
}

glm::mat4 Project::Model::getShearing() {
    return _shear;
}

int Project::Model::getVertexBufferObject() {
    return _vaoRef;
}

void Project::Model::setVertexBufferObject(int vao) {
    _vaoRef = vao;
}

GLuint Project::Model::getTextureID() {
    return _textureID;
}

void Project::Model::setTextureID(GLuint newTextureID) {
    _textureID = newTextureID;
}