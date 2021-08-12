#include "TextureManager.h"
#include <iostream>

std::map<std::string, GLuint> Project::TextureManager::_textures = std::map<std::string, GLuint>();

void Project::TextureManager::instantiate() {
    std::cout << "INFO - TextureManager - instantiate - Starting up." << std::endl;
    Project::TextureManager::loadTexture("default.jpg");
    Project::TextureManager::loadTexture("tiles.jpg");
    Project::TextureManager::loadTexture("metal.jpg");
    Project::TextureManager::loadTexture("box.jpg");
    Project::TextureManager::loadTexture("stage.jpg");
    Project::TextureManager::loadTexture("blue_cube_texture.jpg");
    Project::TextureManager::loadTexture("cat_cube_texture.jpg");
    Project::TextureManager::loadTexture("dog_cube_texture.jpg");
    Project::TextureManager::loadTexture("soccer_cube_texture.jpg");
    Project::TextureManager::loadTexture("pi_cube_texture.jpg");
    Project::TextureManager::loadTexture("house_cube_texture.jpg");
}

GLuint Project::TextureManager::loadTexture(const char* name) {
    std::string fileName = std::string(name);
    if (_textures.find(fileName) != _textures.end()) {
        return _textures.at(fileName);
    }

    // Generate and bind texture
    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    if (textureId == NULL) {
        std::cerr << "ERROR - TextureManager - loadTexture - Texture Generation Error." << std::endl;
        return NULL;
    }
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Set filter parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load textures with dimension data
    int width, height, nrChannels;
    std::string filePath = "data/textures/" + std::string(name);
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cerr << "ERROR - TextureManager - loadTexture - Texture Loading: " << name << std::endl;
        return NULL;
    }

    // Upload the texture to the PU
    GLenum format = 0;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;
    else {
        std::cerr << "ERROR - TextureManager - loadTexture - Unknown Number of Channels: " << nrChannels << std::endl;
        return NULL;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    // Free resources
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    _textures.insert(std::make_pair(name, textureId));
    std::cout << "INFO - TextureManager - loadTexture - Complete: " << name << "." << std::endl;
    return textureId;
}

GLuint Project::TextureManager::getTexture(const char* name) {
    std::string fileName = std::string(name);
    if (_textures.find(fileName) != _textures.end()) {
        return _textures.at(fileName);
    }
    return NULL;
}