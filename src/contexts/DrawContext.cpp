#include "DrawContext.h"
#include "ShaderManager.h"
#include <iostream>


Project::DrawContext::DrawContext(GLFWwindow* window) {
    _window = window;
    _shaderProgram = Project::ShaderManager::get("main");
}

Project::DrawContext::DrawContext(GLFWwindow* window, const char* name) {
    _window = window;
    _shaderProgram = Project::ShaderManager::get(name);
}

/**
 * Returns the window from which the drawing should be done on.
 **/
GLFWwindow* Project::DrawContext::getWindow() {
    return _window;
}

int Project::DrawContext::getShaderProgram() {
    glUseProgram(_shaderProgram);
    return _shaderProgram;
}

void Project::DrawContext::setShaderProgram(const char* name) {
    _shaderProgram = Project::ShaderManager::get(name);
}