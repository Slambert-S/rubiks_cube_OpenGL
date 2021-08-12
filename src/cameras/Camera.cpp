#include "Camera.h"
#include <algorithm>

/**
 * Camera has two main movements:
 *  - Pan (in all 4 directions)
 *  - Tilt (rotation)
 **/
Project::Camera::Camera(glm::vec3 position, glm::vec3 lookAt, glm::vec3 up, GLuint location, bool circularMovement) {
	_position = _defaultPosition = position;
	_lookAt = _defaultLookAt = lookAt;
	_up = _defaultUp = up;
    _cameraMatrixLocation = location;
    _cameraMatrix = glm::lookAt(_position, _position + _lookAt, _up);

    _circularMovement = circularMovement;
    _radius = 15.0f;

    _cameraAngularSpeed = 60.0f;
    _cameraSpeed = 1.0f;
    _cameraHorizontalAngle = 90.0f;
    _cameraVerticalAngle = 0.0f;
    _prevFrameTime = glfwGetTime();
    _prevMousePosX = 0;
    _prevMousePosY = 0;

    MAX_ZOOM = 100.0f;
    MIN_ZOOM = 10.0f;
    _fieldOfViewAngle = 45.0f;
}

/**
* Initialize the camera matrix in default position
* 
* @param context The drawing context at the time of setting up.
**/
void Project::Camera::setupCamera(DrawContext context) {
    glUniformMatrix4fv(_cameraMatrixLocation, 1, GL_FALSE, &_cameraMatrix[0][0]);
    // Set initial view matrix
    glfwGetCursorPos(context.getWindow(), &_prevMousePosX, &_prevMousePosY);
}

/**
* Changes the location of camera.
* 
* @param location the adress for the matrix.
**/
void Project::Camera::setCameraMatrixLocation(GLuint location) {
    _cameraMatrixLocation = location;
}

/**
* Move the camera to the right based on camera speed.
**/
void Project::Camera::moveRight() {
    if (_circularMovement) {
        float dt = 0.02;
        _prevFrameTime += dt;

        _position.x = sin(_cameraSpeed * _prevFrameTime) * _radius;
        _position.z = cos(_cameraSpeed * _prevFrameTime) * _radius;
    }
    else {
        float dt = glfwGetTime() - _prevFrameTime;
        _prevFrameTime += dt;
        _position.x += _cameraSpeed * dt;
    }
}

/**
* Move the camera to the left based on camera speed.
**/
void Project::Camera::moveLeft() {
    if (_circularMovement) {
        float dt = 0.02;
        _prevFrameTime -= dt;

        _position.x = sin(_cameraSpeed * _prevFrameTime) * _radius;
        _position.z = cos(_cameraSpeed * _prevFrameTime) * _radius;
    }
    else {
        float dt = glfwGetTime() - _prevFrameTime;
        _prevFrameTime += dt;
        _position.x -= _cameraSpeed * dt;
    }
}

/**
* Move the camera up based on camera speed.
**/
void Project::Camera::moveUp() {
    if (_circularMovement) {
        float dt = 0.02;
        _prevFrameTime += dt;

       // _position.z = sin(_cameraSpeed * _prevFrameTime) * _radius;
        _position.y = cos(_cameraSpeed * _prevFrameTime) * _radius;
    }
    else {
        float dt = glfwGetTime() - _prevFrameTime;
        _prevFrameTime += dt;
        _position.y += _cameraSpeed * dt;
    }
}

/**
* Move the camera down based on camera speed.
**/
void Project::Camera::moveDown() {
    if (_circularMovement) {
        float dt = 0.02;
        _prevFrameTime -= dt;

       // _position.z = sin(_cameraSpeed * _prevFrameTime) * _radius;
        _position.y = cos(_cameraSpeed * _prevFrameTime) * _radius;
    }
    else {
        float dt = glfwGetTime() - _prevFrameTime;
        _prevFrameTime += dt;
        _position.y -= _cameraSpeed * dt;
    }
}

/**
 * Move the camera forward based on camera speed.
 **/
void Project::Camera::moveForward() {
    float dt = glfwGetTime() - _prevFrameTime;
    _prevFrameTime += dt;
    _position.z += _cameraSpeed * dt;
}

/**
 * Move the camera backward based on camera speed.
 **/
void Project::Camera::moveBackward() {
    float dt = glfwGetTime() - _prevFrameTime;
    _prevFrameTime += dt;
    _position.z -= _cameraSpeed * dt;
}

/**
* Resets the position and orientation of the camera.
**/
void Project::Camera::resetPosition() {

    _position = _defaultPosition;
    _lookAt = _defaultLookAt;
    _up = _defaultUp;

    _cameraHorizontalAngle = 90.0f;
    _cameraVerticalAngle = 0.0f;
    _fieldOfViewAngle = 45.0f;
}

/**
* Returns the angle for the field of view.
* 
* @return angle of field of view.
**/
float Project::Camera::getFieldOfView() {
    return _fieldOfViewAngle;
}

/**
* Move based on mouse interaction given.
* 
* @param context The drawing context at the time of setting up.
* @param handler The handler being used. 
**/
void Project::Camera::calculatePosition(Project::DrawContext context, Project::MouseButtonHandler* handler) {
    float dt = glfwGetTime() - _prevFrameTime;
    if (!_circularMovement)
        _prevFrameTime += dt;

    // Current Mouse Positions
    double mousePosX, mousePosY;
    glfwGetCursorPos(context.getWindow(), &mousePosX, &mousePosY);

    // Change in pos
    double dx = mousePosX - _prevMousePosX;
    double dy = mousePosY - _prevMousePosY;

    // Update Prev
    _prevMousePosX = mousePosX;
    _prevMousePosY = mousePosY;
    
    // Only pan/tilt/zoom on button press
    if (handler->getMiddlePressed() && !_circularMovement) {
        // Tilt
        _cameraVerticalAngle -= dy * _cameraAngularSpeed * dt;
    }
    if (handler->getRightPressed() && !_circularMovement) {
        // Pan
        _cameraHorizontalAngle -= dx * _cameraAngularSpeed * dt;
    }
    if (handler->getLeftPressed()) {
        // Zoom
        if (_circularMovement) {
            _fieldOfViewAngle -= dy * _cameraAngularSpeed * 0.02;
        }
        else {
            _fieldOfViewAngle -= dy * _cameraAngularSpeed * dt;
        }

        if (_fieldOfViewAngle < MIN_ZOOM)
            _fieldOfViewAngle = MIN_ZOOM;
        if (_fieldOfViewAngle > MAX_ZOOM)
            _fieldOfViewAngle = MAX_ZOOM;
    }

    // Clamp vertical angle to [-85, 85] degrees
    _cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, _cameraVerticalAngle));

    float theta = glm::radians(_cameraHorizontalAngle);
    float phi = glm::radians(_cameraVerticalAngle);

    float X = sin(glfwGetTime()) * _radius;
    float Z = cos(glfwGetTime()) * _radius;

    _lookAt = normalize(glm::vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta)));
    glm::vec3 cameraSideVector = glm::cross(_lookAt, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::normalize(cameraSideVector);
    _up = glm::normalize(glm::cross(cameraSideVector, _lookAt));
    _cameraMatrix = glm::lookAt(_position, _position + _lookAt, _up);
    _cameraMatrix = (_circularMovement) ? glm::lookAt(_position, _lookAt, _defaultUp) : glm::lookAt(_position, _position + _lookAt, _up);
    glUniformMatrix4fv(_cameraMatrixLocation, 1, GL_FALSE, &_cameraMatrix[0][0]);
}

glm::vec3 Project::Camera::getCameraPosition() {
    return _position;
}

glm::mat4 Project::Camera::getCameraMatrix() {
    return glm::lookAt(_position, _position + _lookAt, _up);
}

float Project::Camera::getRadius() {
    return _radius;
}

void Project::Camera::setRadius(float radius) {
    _radius = radius;
}
