#include "MouseButtonHandler.h"

/**
 * This handles all mouse button events
 **/
Project::MouseButtonHandler::MouseButtonHandler(DrawContext context) {
    _rightPressed = false;
    _leftPressed = false;
    _middlePressed = false;
    _context = context;
}

void Project::MouseButtonHandler::handle(GLFWwindow* window, int button, int action, int mods) {
     switch (button) {
        case GLFW_MOUSE_BUTTON_RIGHT:
            _rightPressed = setIsPressed(action);
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            _middlePressed = setIsPressed(action);
            break;
        case GLFW_MOUSE_BUTTON_LEFT:
            _leftPressed = setIsPressed(action);
            break;
    }
}

bool Project::MouseButtonHandler::setIsPressed(int action) {
    if (action == GLFW_PRESS) {
        return true;
    }

    return false;
}

bool Project::MouseButtonHandler::getRightPressed() {
    return _rightPressed;
}

bool Project::MouseButtonHandler::getLeftPressed() {
    return _leftPressed;
}

bool Project::MouseButtonHandler::getMiddlePressed() {
    return _middlePressed;
}
