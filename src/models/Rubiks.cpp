#include "Model.h"
#include "Cubelet.h"
#include "TextureManager.h"
#include "Rubiks.h"
#include <map>
#include <iostream>
#include <thread>
#include <chrono>


/**
 * Ideas:
 *  - Camera Rotation is around the rubiks cube
 *  - Limited translations (cube must be visible at all times)
 *  - Rotating rubiks cube is based on camera position and rubiks cube's orientation
 **/
Rubiks::Rubiks(int size, RubiksSettings settings) : Project::Model::Model(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)) {
    _size = size;
    _settings = &settings;
    _animation = NULL;

    _animating = false;

    // Create all cubelets.
    _rubiks = new Cubelet[_size * _size *_size];

    // Initialize the values in the cubelets.
    int defaultTexture = Project::TextureManager::getTexture("default.jpg");
    glm::vec3 cubeSizes = (settings.size - float(_size - 1) * settings.margin) / float(_size);
    for (int row = 0; row < _size; row++) {
        for (int col = 0; col < _size; col++) {
            for (int depth = 0; depth < _size; depth++) {
                Cubelet *cube = &_rubiks[row * _size * _size + col * _size + depth];
                glm::vec3 position = glm::vec3((col + 0.5f) * cubeSizes.x + col * settings.margin.x, (row + 0.5f) * cubeSizes.y + row * settings.margin.y, (depth + 0.5f) * cubeSizes.z + depth * settings.margin.z) - 0.5f * settings.size;

                cube->setPosition(position);
                cube->setRubiksPosition(glm::vec3(float(col), float(row), float(depth)));
                cube->setScaling(cubeSizes);

                cube->setTextureAt(Side::Top, defaultTexture);
                cube->setTextureAt(Side::Bottom, defaultTexture);
                cube->setTextureAt(Side::Left, defaultTexture);
                cube->setTextureAt(Side::Right, defaultTexture);
                cube->setTextureAt(Side::Front, defaultTexture);
                cube->setTextureAt(Side::Back, defaultTexture);

                std::vector<Side> sides = getVisibleSides(col, row, depth);
                for (int s = 0; s < sides.size(); s++)
                    cube->setTextureAt(sides[s], _settings->sides[sides[s]].texture);

                addChild(cube);
            }
        }
    }
}

/**
 * Prints details regarding the rubik's cube to standard output.
 **/
void Rubiks::print() {
    for (int i = 0; i < _size * _size * _size; i++) {
        glm::vec3 position = _rubiks[i].getPosition();
        glm::vec3 rposition = _rubiks[i].getRubiksPosition();

        std::vector<Side> sides = getVisibleSides(rposition.x, rposition.y, rposition.z);
        for (int s = 0; s < sides.size(); s++) {
            std::cout << i << " (" << rposition.x << ", " << rposition.y << ", " << rposition.z << ") " << _rubiks[i].getTextureAt(sides[s]) << std::endl;
        }
    }
}

void Rubiks::rotateColumnDown(int column, bool animation) {
    std::vector<Cubelet*> cubes = getColumn(column);
    rotateColumn(cubes, glm::vec3(0.0f, glm::radians(90.0f), glm::radians(90.0f)), animation);
}

void Rubiks::rotateColumnUp(int column, bool animation) {
    std::vector<Cubelet*> cubes = getColumn(column);
    rotateColumn(cubes, glm::vec3(0.0f, glm::radians(-90.0f), glm::radians(-90.0f)), animation);
}

void Rubiks::rotateRowLeft(int row, bool animation) {
    std::vector<Cubelet*> cubes = getRow(row);
    rotateRow(cubes, glm::vec3(glm::radians(90.0f), 0.0f, glm::radians(90.0f)), animation);
}

void Rubiks::rotateRowRight(int row, bool animation) {
    std::vector<Cubelet*> cubes = getRow(row);
    rotateRow(cubes, glm::vec3(glm::radians(-90.0f), 0.0f, glm::radians(-90.0f)), animation);
}

void Rubiks::rotateDepthLeft(int depth, bool animation) {
    std::vector<Cubelet*> cubes = getDepth(depth);
    rotateDepth(cubes, glm::vec3(glm::radians(-90.0f), glm::radians(-90.0f), 0.0f), animation);
}

void Rubiks::rotateDepthRight(int depth, bool animation) {
    std::vector<Cubelet*> cubes = getDepth(depth);
    rotateDepth(cubes, glm::vec3(glm::radians(90.0f), glm::radians(90.0f), 0.0f), animation);
}


void Rubiks::rotateCubeRight(bool animation) {
    std::vector<Cubelet*> cubes;
    for (int i = 0; i < _size * _size * _size; i++)
        cubes.push_back(&_rubiks[i]);
    rotateRow(cubes, glm::vec3(glm::radians(-90.0f), 0.0f, glm::radians(-90.0f)), animation);
        
}

void Rubiks::rotateCubeLeft(bool animation) {
    std::vector<Cubelet*> cubes;
    for (int i = 0; i < _size * _size * _size; i++)
        cubes.push_back(&_rubiks[i]);
    rotateRow(cubes, glm::vec3(glm::radians(90.0f), 0.0f, glm::radians(90.0f)), animation);

}

void Rubiks::rotateCubeUp(bool animation) {
    std::vector<Cubelet*> cubes;
    for (int i = 0; i < _size * _size * _size; i++)
        cubes.push_back(&_rubiks[i]);
    rotateColumn(cubes, glm::vec3(0.0f, glm::radians(-90.0f), glm::radians(-90.0f)), animation);

}

void Rubiks::rotateCubeDown(bool animation) {
    std::vector<Cubelet*> cubes;
    for (int i = 0; i < _size * _size * _size; i++)
        cubes.push_back(&_rubiks[i]);
    rotateColumn(cubes, glm::vec3(0.0f, glm::radians(90.0f), glm::radians(90.0f)), animation);

}

std::vector<Cubelet*> Rubiks::getColumn(int col) {
    std::vector<Cubelet*> list;
    for (int i = 0; i < _size * _size * _size; i++) {
        if (_rubiks[i].getRubiksPosition().x == col) {
            list.push_back(&_rubiks[i]);
        }
    }
    return list;
}

std::vector<Cubelet*> Rubiks::getDepth(int depth) {
    std::vector<Cubelet*> list;
    for (int i = 0; i < _size * _size * _size; i++) {
        if (_rubiks[i].getRubiksPosition().z == depth) {
            list.push_back(&_rubiks[i]);
        }
    }
    return list;
}

std::vector<Cubelet*> Rubiks::getRow(int row) {
    std::vector<Cubelet*> list;
    for (int i = 0; i < _size * _size * _size; i++) {
        if (_rubiks[i].getRubiksPosition().y == row) {
            list.push_back(&_rubiks[i]);
        }
    }
    return list;
}

bool Rubiks::hasOngoingAnimation() {
    return _animation != NULL;
}

void Rubiks::animate() {
    if (_animation == NULL) {
        return;
    }

    double time = glfwGetTime();
    float delta = (float) glm::min((time - _animation->start) / _animation->duration, 1.0);
    glm::vec3 rotation = _animation->rotation * delta;

    glm::vec3 normalRotation = glm::normalize(rotation);
    glm::vec3 rotationAxis = glm::vec3(rotation.x == 0.0f ? 1.0f : 0.0f, rotation.y == 0.0f ? 1.0f : 0.0f, rotation.z == 0.0f ? 1.0f : 0.0f);
    float rotationSign = rotation.x >= 0.0f && rotation.y >= 0.0f && rotation.z >= 0.0f ? -1.0f : 1.0f;

    if (_animation->start + _animation->duration > time) {
        for (int i = 0; i < _animation->cubeStates.size(); i++) {
            CubeState *state = _animation->cubeStates[i];
            Cubelet *cube = state->cube;

            float angle = fmod(getAngle(state->initialPosition, rotationAxis) + glm::pi<float>() * 2.0f, glm::pi<float>() * 2.0f);;
            float radius = glm::length(state->initialPosition * (glm::vec3(1.0f) - rotationAxis));
            glm::vec3 position = radius * calculateAngularMovement(state->initialPosition, rotation, angle);
            cube->setPosition(position + rotationAxis * state->initialPosition);
            cube->setRotation(state->initialRotation + glm::length(rotation) / sqrt(2.0f) * rotationAxis * rotationSign * delta);
        }
    } else {
        // Make sure all values are correct.
        for (int i = 0; i < _animation->cubeStates.size(); i++) {
            CubeState *state = _animation->cubeStates[i];
            Cubelet *cube = state->cube;
            cube->setPosition(state->initialPosition + state->movement);
            cube->setRotation(state->initialRotation + glm::length(_animation->rotation) / sqrt(2.0f) * rotationAxis * rotationSign);
        }
        
        delete _animation;
        _animation = NULL;
        _animating = false;
    }
}

float Rubiks::getAngle(glm::vec3 position, glm::vec3 rotation) {
    float opposite = rotation.y == 1.0f ? position.z : position.y;
    float near = rotation.x == 1.0f ? position.z : position.x;
    return near < 0.0f ? glm::radians(180.0f) - glm::atan(opposite / near) : glm::atan(opposite / near);
}

glm::vec3 Rubiks::calculateAngularMovement(glm::vec3 position, glm::vec3 rotation, float initialAngle) {
    if (rotation.x == 0.0f) {
        return glm::vec3(0.0f, glm::sin(initialAngle + rotation.y), glm::cos(initialAngle + rotation.z));
    } else if (rotation.y == 0.0f) {
        return glm::vec3(glm::cos(initialAngle + rotation.x), 0.0f, glm::sin(initialAngle + rotation.z));
    } else {
        return glm::vec3(glm::cos(initialAngle + rotation.x), glm::sin(initialAngle + rotation.y), 0.0f);
    }
}

/**
 * Rotates the Rubik's position of each cubelet and starts the animation in order.
 * Rotation cannot happen if there is an animation ongoing.
 * 
 * @param cubes list of cubes to rotate.
 * @param rotation the angle in degrees to rotate for each axis.
 * @param animation toggle for having animations or not.
 **/
void Rubiks::rotateColumn(std::vector<Cubelet*> cubes, glm::vec3 rotation, bool animation) {
    if (_animation != NULL) {
        return;
    }

    std::vector<glm::vec3> iRubiksPositions = std::vector<glm::vec3>();
    for (int i = 0; i < cubes.size(); i++) {
        iRubiksPositions.push_back(cubes[i]->getRubiksPosition());
    }

    // Calculate new rubiks position
    glm::vec3 offset = glm::vec3(float(_size - 1) / 2.0f);
    for (int i = 0; i < cubes.size(); i++) {
        glm::vec3 position = cubes[i]->getRubiksPosition();
        auto temp = position.x;
        glm::vec3 diff = glm::vec3(0.0f, position.y - offset.y, position.z - offset.z);
        float angle = fmod(glm::atan(diff.y, diff.z) + glm::pi<float>() * 2.0f, glm::pi<float>() * 2.0f);
        position = glm::length(diff) * glm::vec3(0.0f, glm::sin(angle + rotation.y), glm::cos(angle + rotation.z)) + offset;
        position = glm::vec3(temp, int(glm::round(position.y)), int(glm::round(position.z)));
        cubes[i]->setRubiksPosition(position);
    }

    std::vector<glm::vec3> deltaMovements = std::vector<glm::vec3>();
    for (int i = 0; i < cubes.size(); i++) {
        glm::vec3 iPos = getRubiksPosition(iRubiksPositions[i].x, iRubiksPositions[i].y, iRubiksPositions[i].z);
        glm::vec3 fPos = getRubiksPosition(cubes[i]->getRubiksPosition().x, cubes[i]->getRubiksPosition().y, cubes[i]->getRubiksPosition().z);
        deltaMovements.push_back(glm::vec3(fPos.x - iPos.x, fPos.y - iPos.y, fPos.z - iPos.z));
    }

    if (animation) {
        int start = glfwGetTime();
        startAnimation(cubes, deltaMovements, rotation);
    } else {
        glm::vec3 rotationAxis = glm::vec3(rotation.x == 0.0f ? 1.0f : 0.0f, rotation.y == 0.0f ? 1.0f : 0.0f, rotation.z == 0.0f ? 1.0f : 0.0f);
        float rotationSign = rotation.x >= 0.0f && rotation.y >= 0.0f && rotation.z >= 0.0f ? -1.0f : 1.0f;
        for (int i = 0; i < cubes.size(); i++) {
            cubes[i]->setPosition(cubes[i]->getPosition() + deltaMovements[i]);
            cubes[i]->setRotation(cubes[i]->getRotation() + glm::length(rotation) / sqrt(2.0f) * rotationAxis * rotationSign);
        }
    }
}

/**
 * Rotates the Rubik's position of each cubelet and starts the animation in order.
 * Rotation cannot happen if there is an animation ongoing.
 * 
 * @param cubes list of cubes to rotate.
 * @param rotation the angle in degrees to rotate for each axis.
 * @param animation toggle for having animations or not.
 **/
void Rubiks::rotateDepth(std::vector<Cubelet*> cubes, glm::vec3 rotation, bool animation) {
    if (_animation != NULL) {
        return;
    }

    std::vector<glm::vec3> iRubiksPositions;
    for (int i = 0; i < cubes.size(); i++) {
        iRubiksPositions.push_back(cubes[i]->getRubiksPosition());
    }

    // Calculate new rubiks position
    glm::vec3 offset = glm::vec3(float((_size - 1) / 2));
    for (int i = 0; i < cubes.size(); i++) {
        glm::vec3 position = cubes[i]->getRubiksPosition();
        auto temp = position.z;
        glm::vec3 diff = glm::vec3(position.x - offset.x, position.y - offset.y, 0.0f);
        float angle = fmod(glm::atan(diff.y, diff.x) + glm::pi<float>() * 2.0f, glm::pi<float>() * 2.0f);
        position = glm::length(diff) * glm::vec3(glm::cos(angle + rotation.x), glm::sin(angle + rotation.y), 0.0f) + offset;
        position = glm::vec3(int(glm::round(position.x)), int(glm::round(position.y)), temp);
        cubes[i]->setRubiksPosition(position);
    }

    std::vector<glm::vec3> deltaMovements;
    for (int i = 0; i < cubes.size(); i++) {
        glm::vec3 iPos = getRubiksPosition(iRubiksPositions[i].x, iRubiksPositions[i].y, iRubiksPositions[i].z);
        glm::vec3 fPos = getRubiksPosition(cubes[i]->getRubiksPosition().x, cubes[i]->getRubiksPosition().y, cubes[i]->getRubiksPosition().z);
        deltaMovements.push_back(fPos - iPos);
    }

    if (animation) {
        int start = glfwGetTime();
        startAnimation(cubes, deltaMovements, rotation);
    } else {
        glm::vec3 rotationAxis = glm::vec3(rotation.x == 0.0f ? 1.0f : 0.0f, rotation.y == 0.0f ? 1.0f : 0.0f, rotation.z == 0.0f ? 1.0f : 0.0f);
        float rotationSign = rotation.x >= 0.0f && rotation.y >= 0.0f && rotation.z >= 0.0f ? -1.0f : 1.0f;
        for (int i = 0; i < cubes.size(); i++) {
            cubes[i]->setPosition(cubes[i]->getPosition() + deltaMovements[i]);
            cubes[i]->setRotation(cubes[i]->getRotation() - glm::length(rotation) / sqrt(2.0f) * rotationAxis * rotationSign);
        }
    }
}

/**
 * Rotates the Rubik's position of each cubelet and starts the animation in order.
 * Rotation cannot happen if there is an animation ongoing.
 * 
 * @param cubes list of cubes to rotate.
 * @param rotation the angle in degrees to rotate for each axis.
 * @param animation toggle for having animations or not.
 **/
void Rubiks::rotateRow(std::vector<Cubelet*> cubes, glm::vec3 rotation, bool animation) {
    if (_animation != NULL) {
        return;
    }

    std::vector<glm::vec3> iRubiksPositions;
    for (int i = 0; i < cubes.size(); i++) {
        iRubiksPositions.push_back(cubes[i]->getRubiksPosition());
    }

    // Calculate new rubiks position
    glm::vec3 offset = glm::vec3(float((_size - 1) / 2));
    for (int i = 0; i < cubes.size(); i++) {
        glm::vec3 position = cubes[i]->getRubiksPosition();
        auto temp = position.y;
        glm::vec3 diff = glm::vec3(position.x - offset.x, 0.0f, position.z - offset.z);
        float angle = fmod(glm::atan(diff.z, diff.x) + glm::pi<float>() * 2.0f, glm::pi<float>() * 2.0f);
        position = glm::length(diff) * glm::vec3(glm::cos(angle + rotation.x), 0.0f, glm::sin(angle + rotation.z)) + offset;
        position = glm::vec3(int(glm::round(position.x)), temp, int(glm::round(position.z)));
        cubes[i]->setRubiksPosition(position);
    }

    std::vector<glm::vec3> deltaMovements;
    for (int i = 0; i < cubes.size(); i++) {
        glm::vec3 iPos = getRubiksPosition(iRubiksPositions[i].x, iRubiksPositions[i].y, iRubiksPositions[i].z);
        glm::vec3 fPos = getRubiksPosition(cubes[i]->getRubiksPosition().x, cubes[i]->getRubiksPosition().y, cubes[i]->getRubiksPosition().z);
        deltaMovements.push_back(fPos - iPos);
    }

    if (animation) {
        int start = glfwGetTime();
        startAnimation(cubes, deltaMovements, rotation);
    } else {
        glm::vec3 rotationAxis = glm::vec3(rotation.x == 0.0f ? 1.0f : 0.0f, rotation.y == 0.0f ? 1.0f : 0.0f, rotation.z == 0.0f ? 1.0f : 0.0f);
        float rotationSign = rotation.x >= 0.0f && rotation.y >= 0.0f && rotation.z >= 0.0f ? -1.0f : 1.0f;
        for (int i = 0; i < cubes.size(); i++) {
            cubes[i]->setPosition(cubes[i]->getPosition() + deltaMovements[i]);
            cubes[i]->setRotation(cubes[i]->getRotation() + glm::length(rotation) / sqrt(2.0f) * rotationAxis * rotationSign);
        }
    }
}

std::vector<Side> Rubiks::getVisibleSides(int column, int row, int depth) {
    std::vector<Side> sides;

    if (row == 0)
        sides.push_back(Side::Bottom);
    if (column == 0)
        sides.push_back(Side::Left);
    if (column == _size - 1)
        sides.push_back(Side::Right);
    if (row == _size - 1)
        sides.push_back(Side::Top);
    if (depth == 0)
        sides.push_back(Side::Back);
    if (depth == _size - 1)
        sides.push_back(Side::Front);
    return sides;
}

glm::vec3 Rubiks::getRubiksPosition(int column, int row, int depth) {
    glm::vec3 cubeSizes = (_settings->size - float(_size - 1) * _settings->margin) / float(_size);
    return glm::vec3((column + 0.5f) * cubeSizes.x + column * _settings->margin.x, (row + 0.5f) * cubeSizes.y + row * _settings->margin.y, (depth + 0.5f) * cubeSizes.z + depth * _settings->margin.z) - 0.5f * _settings->size;
}

void Rubiks::startAnimation(std::vector<Cubelet*> cubes, std::vector<glm::vec3> movements, glm::vec3 rotation) {
    std::vector<CubeState*> states;
    for (int i = 0; i < cubes.size(); i++) {
        states.push_back(new CubeState(cubes[i], movements[i]));
    }

    _animation = new AnimationDetails { states, rotation, glfwGetTime(), _settings->rotation_animation_time };
}
void Rubiks::setAnimating(bool a) {
    _animating = a;
}

bool Rubiks::getAnimating() {
    return _animating;
}

void Rubiks::drawModel(Project::DrawContext context, glm::mat4 transformations) {
    animate();
}

int Rubiks::generateVertexBufferObject() {
    return -1;
}