#include "ShaderManager.h"
#include <iostream>
#include <fstream>


std::map<std::string, const char*> Project::ShaderManager::_shaders = std::map<std::string, const char*>();
std::map<std::string, GLuint> Project::ShaderManager::_programs = std::map<std::string, GLuint>();


GLuint Project::ShaderManager::compile(const char* name, const char* vertex, const char* fragment, const char* geometry) {
    if (_programs.find(name) != _programs.end()) {
        return _programs.at(name);
    }

    const char* vertexShaderSource = getVertexShader(vertex);
    const char* fragmentShaderSource = getFragmentShader(fragment);
    const char* geometryShaderSource = geometry == NULL ? NULL : getGeometryShader(geometry);
    GLuint shaderProgram = compileShader(vertexShaderSource, fragmentShaderSource, geometryShaderSource);
    std::cout << "INFO - ShaderManager - Compile - Complete: Compiled shader program nicknamed " << name << "." << std::endl;

    _programs.insert(std::make_pair(name, shaderProgram));
    return shaderProgram;
}

GLuint Project::ShaderManager::compileShader(const char* vertexShaderSource, const char* fragmentShaderSource, const char* geometryShaderSource) {
    glEnable(GL_DEPTH_TEST);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint success = GL_FALSE;
    char log[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE) {
        glGetShaderInfoLog(vertexShader, 512, NULL, log);
        std::cout << "ERROR - Shader - Vertex - Compile Failure:\n" << log << std::endl;
        return -1;
    }

    success = GL_FALSE;
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, log);
        std::cout << "ERROR - Shader - Fragment - Compile Failure:\n" << log << std::endl;
        return -1;
    }

    unsigned int geometryShader;
    if (geometryShaderSource != NULL) {
        success = GL_FALSE;
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
        glCompileShader(geometryShader);

        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
        if (success != GL_TRUE) {
            glGetShaderInfoLog(geometryShader, 512, NULL, log);
            std::cout << "ERROR - Shader - Geometry - Compile Failure:\n" << log << std::endl;
            return -1;
        }
    }

    success = GL_FALSE;
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    if (geometryShaderSource != NULL)
        glAttachShader(shaderProgram, geometryShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, log);
        std::cout << "ERROR - Shader - Program - Compiled Failed:\n" << log << std::endl;
        return -1;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (geometryShaderSource != NULL)
        glDeleteShader(geometryShader);

    return shaderProgram;
}

GLuint Project::ShaderManager::get(const char* name) {
    if (_programs.find(name) != _programs.end()) {
        return _programs.at(name);
    }

    return 0;
}

const char* Project::ShaderManager::getFragmentShader(const char* name) {
    return Project::ShaderManager::read("data/shaders/fragment/" + std::string(name));
}

const char* Project::ShaderManager::getVertexShader(const char* name) {
    return Project::ShaderManager::read("data/shaders/vertex/" + std::string(name));
}

const char* Project::ShaderManager::getGeometryShader(const char* name) {
    return Project::ShaderManager::read("data/shaders/geometry/" + std::string(name));
}

void Project::ShaderManager::instantiate() {
    std::cout << "INFO - ShaderManager - instantiate - Starting up." << std::endl;
    Project::ShaderManager::compile("main", "cube", "cube");
    Project::ShaderManager::compile("depth", "simpleDepthShader", "simpleDepthShader", "simpleDepthShader");
    Project::ShaderManager::compile("lamp", "spotlight", "spotlight");
}

/**
 * Fetches the content of a file as a char array.
 * 
 * This should never allow user input as argument as it can be exploited.
 * Static classes are bad, but we have no dependency injection.
 **/
const char* Project::ShaderManager::read(std::string name) {
    if (_shaders.find(name) != _shaders.end()) {
        return _shaders.at(name);
    }

    try {
        std::string shader;
        std::fstream file;
        file.open(std::string(name) + ".glsl", std::ios::in);
        
        if (file) {
            std::string content;
            while (getline(file, content)) {
                shader += content + "\n";
            }
            file.close();

            // Copy temporary char buffer into memory.
            char* temp = new char[shader.size() + 1];
            std::copy(shader.begin(), shader.end(), temp);
            temp[shader.size()] = '\0';
            _shaders.insert(std::make_pair(name, temp));
            std::cout << "INFO - ShaderManager - Read - Complete: " << name << std::endl;
            return _shaders.at(name);
        } else {
            std::cout << "WARN - ShaderManager - Read - File: Could not find file named " << name << ".glsl" << std::endl;
        }
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    } catch (const std::string& ex) {
        std::cerr << ex << std::endl;
    } catch (...) {
        std::cerr << "ERROR - ShaderManager - Read - Unhandled Exception." << std::endl;
    }
    return NULL;
}