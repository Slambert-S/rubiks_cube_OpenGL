#include "DrawContext.h"
#include "Model.h"
#include "MouseButtonHandler.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Rubiks.h"
#include "Floor.h"
#include "LightSphere.h"
#include "Stage.h"
#include <iostream>
#include <random>
#include "OIDynamicNullGestureTracker.h"
#include "OIGestureTracker.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H


const char* TITLE = "COMP 371 - Project - Team 3";
const bool DEBUG_MODE = false;

const float circlePosX = 15.8392f;
const float circlePosZ = 15.8392f;
float deltaTime = 0.0f;
float spinSpeed = 0.005f;
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
int width, height;
glm::vec3 scale_vec = glm::vec3(0.0f, 0.0f, 0.0f);
Project::MouseButtonHandler* mouseButtonHandler;
bool textureOn;
openiss::OIGestureTracker* gestureTracker;





GLFWwindow* setup() {
    // Initialize GLFW and OpenGL version
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a 1024x768 window and the rendering context using GLFW.
    GLFWwindow* window = glfwCreateWindow(1024, 768, TITLE, NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return NULL;
    }
    return window;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    mouseButtonHandler->handle(window, button, action, mods);
}

bool shadowToggle = true;
void toggleShadowsCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_N && action == GLFW_PRESS) {
        shadowToggle = !shadowToggle;
    }
}

std::vector<Project::Camera*> setupCameras(Project::DrawContext context) {
    std::vector<Project::Camera*> cameras = std::vector<Project::Camera*>();
    
    glm::vec3 defaultPosition(0.0f, 1.0f, 15.0f);
    glm::vec3 defaultLookAt(0.0f, 0.0f, -1.0f);
    glm::vec3 defaultUp(0.0f, 1.0f, 0.0f);

    cameras.push_back(new Project::Camera(defaultPosition, defaultLookAt, defaultUp, glGetUniformLocation(context.getShaderProgram(), "viewMatrix")));
    
    defaultPosition =glm::vec3(-15.0f, 1.0f, 0.0f);
    cameras.push_back(new Project::Camera(defaultPosition, defaultLookAt, defaultUp, glGetUniformLocation(context.getShaderProgram(), "viewMatrix")));

    defaultPosition = glm::vec3(0.0f, 1.0f, -15.0f);
    cameras.push_back(new Project::Camera(defaultPosition, defaultLookAt, defaultUp, glGetUniformLocation(context.getShaderProgram(), "viewMatrix")));

    defaultPosition = glm::vec3(15.0f, 1.0f, 0.0f);
    cameras.push_back(new Project::Camera(defaultPosition, defaultLookAt, defaultUp, glGetUniformLocation(context.getShaderProgram(), "viewMatrix")));

    defaultPosition = glm::vec3(0.0f, 8.0f, 15.0f);
    cameras.push_back(new Project::Camera(defaultPosition, defaultLookAt, defaultUp, glGetUniformLocation(context.getShaderProgram(), "viewMatrix")));
   
    defaultPosition = glm::vec3(0.0f, -7.0f, -15.0f);
    cameras.push_back(new Project::Camera(defaultPosition, defaultLookAt, defaultUp, glGetUniformLocation(context.getShaderProgram(), "viewMatrix")));

    return cameras;
}

// LIGHTING
// ----------------- LIGHTING --------------------- 
std::string name;
float outerCut = 17.5f;
float innerCut = 12.5f;
float quadratic = 0.0002f;
float constant = 1.0f;
float linear = 0.007f;;

glm::vec3 ambient = glm::vec3(0.2);
glm::vec3 diffuse = glm::vec3(0.5);
glm::vec3 specular = glm::vec3(0.5);
void setupLight(int shader, glm::vec3 position, glm::vec3 direction, bool lightOn) {
    name = "spot_light.";
    if (lightOn) {
        glUniform3fv(glGetUniformLocation(shader, (name + "position").c_str()), 1, &position[0]);
        glUniform3fv(glGetUniformLocation(shader, (name + "direction").c_str()), 1, &direction[0]);

        glUniform1f(glGetUniformLocation(shader, (name + "cutOff").c_str()), glm::cos(glm::radians(innerCut)));
        glUniform1f(glGetUniformLocation(shader, (name + "outerCutOff").c_str()), glm::cos(glm::radians(outerCut)));

        glUniform1f(glGetUniformLocation(shader, (name + "quadratic").c_str()), quadratic);
        glUniform1f(glGetUniformLocation(shader, (name + "constant").c_str()), linear);
        glUniform1f(glGetUniformLocation(shader, (name + "linear").c_str()), linear);

        glUniform3fv(glGetUniformLocation(shader, (name + "ambient").c_str()), 1, &ambient[0]);
        glUniform3fv(glGetUniformLocation(shader, (name + "diffuse").c_str()), 1, &diffuse[0]);
        glUniform3fv(glGetUniformLocation(shader, (name + "specular").c_str()), 1, &specular[0]);
    }
    else {
        glUniform1f(glGetUniformLocation(shader, (name + "cutOff").c_str()), glm::cos(glm::radians(0.0f)));
        glUniform1f(glGetUniformLocation(shader, (name + "outerCutOff").c_str()), glm::cos(glm::radians(0.0f)));
    }
}


// ----------------- SHADOWS -----------------
GLuint depthMap;
GLuint depthMapFBO;

void shadowBuffer() {
    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);

    for (unsigned int i = 0; i < 6; ++i) {

        //GL_TEXTURE_CUBE_MAP_POSITIVE_X = (-Y,-Z) face
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer is incomplete." << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//TODO: function should only be called when cube is not currently animating, not sure how to check that.

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

std::vector<GLuint> textures;
std::map<GLchar, Character> Characters;


unsigned int VAO, VBO;
// render line of text
// -------------------
void RenderText(int shader, std::string text, float x, float y, float scale, glm::vec3 color)
{

    // activate corresponding render state	
    glUniform3f(glGetUniformLocation(shader, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void computeGesture(Rubiks* r, openiss::OIGestureTracker* g) {
    openiss::OIHandData hand = g->getHands()[0];
    openiss::OIGestureData gest = g->getGestures()[0];

    if (hand.isHandLost()) {
        if (gest.getGesturePosition().y > 0.0f && gest.getGesturePosition().y > abs(gest.getGesturePosition().x)) {
            r->rotateCubeUp(true);
        }
        else if (gest.getGesturePosition().x > 0.0f && gest.getGesturePosition().x > abs(gest.getGesturePosition().y)) {
            r->rotateCubeRight(true);
        }
        else if (gest.getGesturePosition().y < 0.0f && abs(gest.getGesturePosition().y) > abs(gest.getGesturePosition().x)) {
            r->rotateCubeDown(true);
        }
        else if (gest.getGesturePosition().x < 0.0f && abs(gest.getGesturePosition().x) > abs(gest.getGesturePosition().y)) {
            r->rotateCubeLeft(true);
        }
        else r->setAnimating(false);
                

        
    }
    else {
        if (hand.getHandPosition().y > 0.0f && hand.getHandPosition().y > abs(hand.getHandPosition().x)) {
            if (hand.getHandPosition().x > 0.0f)
                r->rotateRowRight(2, true);
            else
                r->rotateRowLeft(2, true);
        }
        else if (hand.getHandPosition().x > 0.0f && hand.getHandPosition().x > abs(hand.getHandPosition().y)) {
            if (hand.getHandPosition().y > 0.0f)
                r->rotateColumnUp(2, true);
            else
                r->rotateColumnDown(2, true);

        }
        else if (hand.getHandPosition().y < 0.0f && abs(hand.getHandPosition().y) > abs(hand.getHandPosition().x)) {
            if (hand.getHandPosition().x > 0.0f)
                r->rotateRowRight(0, true);

            else
                r->rotateRowLeft(2, true);
        }
        else if (hand.getHandPosition().x < 0.0f && abs(hand.getHandPosition().x) > abs(hand.getHandPosition().y)){
            if (hand.getHandPosition().y > 0.0f)
                r->rotateColumnUp(0, true);
            else
                r->rotateColumnDown(0, true);
        }
        else r->setAnimating(false);
    }
}

void GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

/*
When call , the function generate randome mouvment on random section of the cube randomly.  csamble the cube without animations. 
*/
void scrambleCube(Rubiks* r) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distrMove(10, 25); // define the range

    std::uniform_int_distribution<> distrAction(0, 7); // define the range
    std::uniform_int_distribution<> distrPart(0, 1); // define the range

    int move = distrMove(gen);
    int target = 0;
    int action = 0;

    for (int i = 1; i <= move; i++) {
        target = distrPart(rd) == 1 ? 2 : 0;
        action = distrAction(rd);

        switch (action) {
        case 0:
            r->rotateColumnUp(target, false);
            break;
        case 1:
            r->rotateColumnDown(target, false);
            break;
        case 2:
            r->rotateRowLeft(target, false);
            break;
        case 3:
            r->rotateRowRight(target, false);
            break;
        case 4:
            r->rotateCubeLeft(false);
            break;
        case 5:
            r->rotateCubeRight(false);
            break;
        case 6:
            r->rotateCubeUp(false);
            break;
        case 7:
            r->rotateCubeDown(false);
            break;
        }
    }
}



int main(int argc, char* argv[])
{
    GLFWwindow* window = setup();
    if (window == NULL) {
        return -1;
    }

    glfwGetFramebufferSize(window, &width, &height);

    gestureTracker = new openiss::OIDynamicNullGestureTracker();
    Project::ShaderManager::instantiate();
    Project::TextureManager::instantiate();
    std::cout << "Loaded. Ready." << std::endl;

    // ----------------- SHADERS -----------------
    int shaderProgram = Project::ShaderManager::get("main");
    auto context = Project::DrawContext(window, "main");
    
    int simpleDepthShaderProgram = Project::ShaderManager::get("depth");
    auto depthContext = Project::DrawContext(window, "depth");

    int lampShaderProgram = Project::ShaderManager::get("lamp");
    auto lampContext = Project::DrawContext(window, "lamp");

    //--------------TEXT SHADER???? HELP ME 
    int textShaderProgram = Project::ShaderManager::compile("font", "text", "text");
    context = Project::DrawContext(window);
    


    // ----------------- Models -----------------
    glm::vec3 defaultScale = glm::vec3(1.0f);
    glm::vec3 defaultShear = glm::vec3(0.0f);
    glm::vec3 centerFront = glm::vec3(0.0f, 0.0f, 9.0f);

    // Stage
    glm::vec3 stagePos(0.0f, -5.5f, 7.0f);
    auto stage = new Stage(stagePos, glm::vec3(0.0f, glm::radians(90.0f), 0.0f), glm::vec3(2.5f));
    stage->setTextureID(Project::TextureManager::getTexture("stage.jpg"));

    // Floor
    glm::vec3 floorPos(0.0f, -5.0f, 0.0f);
    auto floor = new Floor(floorPos, glm::vec3(0.0f), defaultScale);
    floor->setTextureID(Project::TextureManager::getTexture("tiles.jpg"));
    
    // Add to vector models
    std::map<Side, SideInfo> sides;
    sides.insert(std::make_pair(Side::Back, SideInfo{ Project::TextureManager::getTexture("blue_cube_texture.jpg") }));
    sides.insert(std::make_pair(Side::Front, SideInfo { Project::TextureManager::getTexture("pi_cube_texture.jpg") }));
    sides.insert(std::make_pair(Side::Left, SideInfo{ Project::TextureManager::getTexture("cat_cube_texture.jpg") }));
    sides.insert(std::make_pair(Side::Right, SideInfo { Project::TextureManager::getTexture("soccer_cube_texture.jpg") }));
    sides.insert(std::make_pair(Side::Bottom, SideInfo { Project::TextureManager::getTexture("dog_cube_texture.jpg") }));
    sides.insert(std::make_pair(Side::Top, SideInfo{ Project::TextureManager::getTexture("house_cube_texture.jpg") }));
   
    RubiksSettings settings = RubiksSettings { glm::vec3(5.0f), glm::vec3(0.25f), sides, 0.500 };
    Rubiks *r = new Rubiks(3, settings);

    // Light Source
    glm::vec3 lightPos = glm::vec3(0.0f, 20.0f, 30.0f);
    glm::vec3 lightLookAt = glm::vec3(0.0f, -1.0f, -1.0f);
    auto light = new LightSource(lightPos, glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f));

    // ----------------- Camera -----------------
    // Setup for each model
    std::vector<Project::Camera*> cameras = setupCameras(context);

    // Current camera
    auto activeCamera = cameras.at(0);

    // Background
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);

    // --------- Projection Matrix
    float defaultFieldOfView = 70.0f;
    float aspectRatio = 1024.0f / 768.0f;
    float near = 0.01f;
    float far = 100.0f;

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(defaultFieldOfView), aspectRatio, near, far);

    GLuint cameraPositionLocation = glGetUniformLocation(shaderProgram, "cameraPosition");
    GLuint lightDirectionLocation = glGetUniformLocation(shaderProgram, "lightDirection");
    GLuint lightInnerCutoffLocation = glGetUniformLocation(shaderProgram, "light_cutoff_inner");
    GLuint lightOuterCutoffLocation = glGetUniformLocation(shaderProgram, "light_cutoff_outer");
    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    // ----------------- SHADOWS -----------------
    shadowBuffer();
    auto shadowToggleLocation = glGetUniformLocation(shaderProgram, "toggle_shadows");
    glUniform1i(shadowToggleLocation, shadowToggle);

    float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;

    float far_plane = 100.0f;
    float near_plane = 1.0f;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near_plane, far_plane);
    std::vector<glm::mat4> shadowTransforms;
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));



    //variables for frame movement 
    float lastFrameTime = glfwGetTime();
    float movementSpeed = 1.0f;
    float rotationSpeed = 180.0f;  // 180 degrees per second
    float angle = 0;
    
    // ----------------- Input Handling -----------------
    mouseButtonHandler = new Project::MouseButtonHandler(context);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    int lastXButtonState = GLFW_RELEASE;
    int lastSpaceButtonState = GLFW_RELEASE;
    int lastKButtonState = GLFW_RELEASE;
    int lastNButtonState = GLFW_RELEASE;
    int activeFace = 1;
    bool gestureTrakingswitch = true;

    int lastPButtonState = GLFW_RELEASE;
    int lastOButtonState = GLFW_RELEASE;
    glfwSetKeyCallback(window, toggleShadowsCallback);
    //glfwSetKeyCallback(window, toggleLightCallback);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // ----------------- DEBUG -----------------
    if (DEBUG_MODE) {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(DebugMessageCallback, 0);
    }
    setupLight(shaderProgram, lightPos, lightLookAt, true);


    //---------------FREETYPE
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    // find path to font
    std::string font_name = std::string("data/textures/arial.ttf");

    if (font_name.empty())
    {
        std::cout << "POTOTO ERROR::FREETYPE: Failed to load font_name" << std::endl;
        return -1;
    }

    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
        std::cout << "MIRACLE ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
               static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);


    // configure VAO/VBO for texture quads
    // -----------------------------------

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    std::string name;
    

    // variable for timer , intitialisation 
    
    int second = 0;
    int minutes = 0;
    int savedTime = 0;
    int start_time = glfwGetTime();
    bool runningTimer = false;
    std::string timerString = "Timer  ";

    // Entering Main Loop
    while (!glfwWindowShouldClose(window))
    {
        float cameraSpeed = 1.0 * deltaTime;

        //Frame time calculation
        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

       
            


        // Calculate camera pos
        glUseProgram(shaderProgram);
        activeCamera->calculatePosition(context, mouseButtonHandler);
        float newFieldOfView = activeCamera->getFieldOfView();

        // RENDER DEPTH MAP
        glUseProgram(simpleDepthShaderProgram);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(0);

        for (unsigned int i = 0; i < 6; ++i) {
            std::string name = "shadowMatrices[" + std::to_string(i) + "]";
            glUniformMatrix4fv(glGetUniformLocation(simpleDepthShaderProgram, name.c_str()), 1, GL_FALSE, &shadowTransforms[i][0][0]);
        }

        glUniform1f(glGetUniformLocation(simpleDepthShaderProgram, "far_plane"), far_plane);
        glUniform3fv(glGetUniformLocation(simpleDepthShaderProgram, "lightPos"), 1, &lightPos[0]);
        
        // Drawing
        floor->draw(depthContext);
        stage->draw(depthContext);
        r->draw(depthContext);
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // RENDER SCENE WITH SHADOWS
        glUseProgram(shaderProgram);
        projectionMatrix = glm::perspective(glm::radians(newFieldOfView), aspectRatio, near, far);
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightPosition"), 1, &lightPos[0]);
        glUniform3fv(cameraPositionLocation, 1, &activeCamera->getCameraPosition()[0]);

        glUniform1f(glGetUniformLocation(shaderProgram, "far_plane"), far_plane);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);

        //-----------FREETYPE RENDER
        glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
        glUseProgram(textShaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(textShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glActiveTexture(GL_TEXTURE0);
        glClear(GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(0);

        //timer in minutes and second
        timerString = "Timer  ";
        timerString = timerString + std::to_string(minutes);

        /*
         It the timer is running, get the time from GLFW in second.
         convert the second in minutes and set the rest as second
        */
        if (runningTimer) {
            int time = glfwGetTime()- start_time;
            second = (time % 60);
            minutes = (time / 60);
        }

        /*
        if the value of second are under 10 change the display text to add a 0 in front of the value to always display 2 number.
        After display the time on screen as 2D text.
        */
        if (second / 10 == 0) {
            timerString = timerString + ":0" + std::to_string(second);
            RenderText(textShaderProgram, timerString, 570.0f, 570.0f, 0.5f, glm::vec3(0.0, 0.0f, 0.0f));
        }
        else {
            timerString = timerString + ":" + std::to_string(second);
            RenderText(textShaderProgram, timerString, 570.0f, 570.0f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
        }


       
        // Drawing
        floor->draw(context);
        stage->draw(context);
        r->draw(context);
        
        // --------- Shadow toggle
        glUniform1i(shadowToggleLocation, shadowToggle);

        // RENDER LIGHTS
        glUseProgram(lampShaderProgram);

        glUniformMatrix4fv(glGetUniformLocation(lampShaderProgram, "projectionMatrix"), 1, GL_FALSE, &projectionMatrix[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(lampShaderProgram, "viewMatrix"), 1, GL_FALSE, &activeCamera->getCameraMatrix()[0][0]);

        light->draw(lampContext);

        // End frame
        glfwSwapBuffers(window);

        // Detect inputs
        glfwPollEvents();

        // --------- close window
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        //----------Scaling Selected Model
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
            glm::vec3 scale = glm::vec3(0.01f, 0.01f, 0.01f);
            if (scale_vec.x <= 1.0 && scale_vec.y <= 1.0 && scale_vec.z <= 1.0) {
                scale_vec.x += scale.x;
                scale_vec.y += scale.y;
                scale_vec.z += scale.z;
                //activeModel->setScaling(glm::vec3(scale_vec.x, scale_vec.y, scale_vec.z));
            }
            else {
                scale_vec.x = 1.0f;
                scale_vec.y = 1.0f;
                scale_vec.z = 1.0f;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
            // scale down
            glm::vec3 scale = glm::vec3(0.01f, 0.01f, 0.01f);
            if (scale_vec.x >= 0.5 && scale_vec.y >= 0.5 && scale_vec.z >= 0.5) {
                scale_vec.x -= scale.x;
                scale_vec.y -= scale.y;
                scale_vec.z -= scale.z;
                //activeModel->setScaling(glm::vec3(scale_vec.x, scale_vec.y, scale_vec.z));
            }
            else {
                scale_vec.x = 0.5f;
                scale_vec.y = 0.5f;
                scale_vec.z = 0.5f;
            }
        }

        // --------- Render Mode
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
            // Lines
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS) {
            // Points
            
        }

        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
            // Filled 
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        // --------- Camera Movement
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            {
                activeCamera->moveForward();
            }

            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            {
                activeCamera->moveBackward();
            }
        }
        else
        {
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            {
                activeCamera->moveRight();
            }

            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            {
                activeCamera->moveLeft();
            }

            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            {
                activeCamera->moveUp();  
            }

            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            {
                activeCamera->moveDown();
            }
        }

        if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
        {
            // Reset Scale
            // Reset rotation * position

            //reset initial world position
            activeCamera->resetPosition();
            
            activeCamera = cameras.at(0);
        }

        //------face selection

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            
            //reset initial world position
            activeCamera->resetPosition();
            
            activeCamera = cameras.at(0);
        }

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {

            //reset initial world position
            activeCamera->resetPosition();
            
            activeCamera = cameras.at(1);
        }

        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {

            //reset initial world position
            activeCamera->resetPosition();
            
            activeCamera = cameras.at(2);
        }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {

            //reset initial world position
            activeCamera->resetPosition();
            
            activeCamera = cameras.at(3);
        }

        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        {

            //reset initial world position
            activeCamera->resetPosition();
            
            activeCamera = cameras.at(4);
        }

        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
        {

            //reset initial world position
            activeCamera->resetPosition();
            
            activeCamera = cameras.at(2);
        }

         //------ move top Row left
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            

            //pointer &activeFace
            r->rotateRowLeft(2, true);
        }

        //------------- move top row Right
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            
            r->rotateRowRight(2, true);
        }
         //--- Move bottom row leftss
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

            r->rotateRowLeft(0, true);
        }

        //--- Move bottom row right
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

            r->rotateRowRight(0, true);
        }

        //---------Rotate left column up
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

            r->rotateColumnUp(0, true);
        }

        //---------Rotate left column Down
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

            r->rotateColumnDown(0, true);
        }

        //---------Rotate right column up
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {

            r->rotateColumnUp(2, true);
        }

        //---------Rotate right column Down
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {

            r->rotateColumnDown(2, true);
        }

        //---------Rotate middle row left 
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {

            r->rotateRowLeft(1, true);
        }

        //---------Rotate middle row right 
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {

            r->rotateRowRight(1, true);
        }

        //--------Rotate middle column up
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {

            r->rotateColumnUp(1, true);
        }

        //--------Rotate middle column down 
        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {

            r->rotateColumnDown(1, true);
        }

        /*
        //---------Model position and orientation
        // Uppercase WASD translates
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                r->setRotation(glm::vec3(0.0f, 90.0f, 0.0f));
            } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                r->setRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
            } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                r->setRotation(glm::vec3(90.0f, 0.0f, 0.0f));
            } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                r->setRotation(glm::vec3(0.0f, -90.0f, 0.0f));
            }
        }
        else
        {
            if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
                r->rotateRowLeft(0, false);
            } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                r->rotateRowLeft(1, true);
            } else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
                r->rotateRowLeft(2, true);
            } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                r->rotateRowRight(0, true);
            } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                r->rotateRowRight(1, true);
            } else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
                r->rotateRowRight(2, true);
            } else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
                r->rotateColumnUp(0, true);
            } else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
                r->rotateColumnUp(1, true);
            } else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
                r->rotateColumnUp(2, true);
            } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                r->rotateColumnDown(0, true);
            } else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
                r->rotateColumnDown(1, true);
            } else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
                r->rotateColumnDown(2, true);
            }
        }
        */
        
        lastSpaceButtonState = glfwGetKey(window, GLFW_KEY_SPACE);

        //--------------- Toggle Texture on/off
        if (lastXButtonState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        {
            textureOn = !textureOn;
        }
        lastXButtonState = glfwGetKey(window, GLFW_KEY_X);

        //---------------- Scarble the cube randomly 
        if (lastKButtonState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        {
            scrambleCube(r);
            gestureTrakingswitch = false;
            start_time = glfwGetTime();
            runningTimer = true;
        }
        lastKButtonState = glfwGetKey(window, GLFW_KEY_K);

        //GESTURE CONTROLS

        if (lastNButtonState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        {
            gestureTrakingswitch = !gestureTrakingswitch;
        }
        lastNButtonState = glfwGetKey(window, GLFW_KEY_N);

        if (!r->getAnimating()&& gestureTrakingswitch== true) {
            r->setAnimating(true);
            computeGesture(r, gestureTracker);
        }

        //-----------Ccontrolling the timer
       //----------Stop and start
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && lastPButtonState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            runningTimer = !runningTimer;
            if (runningTimer) {
                std::cout << "timer started" << std::endl;
                start_time = start_time + (glfwGetTime() -savedTime);
                
            }
            else {
                std::cout << "timer stopped" << std::endl;
                savedTime = glfwGetTime();
            }
        }
        lastPButtonState = glfwGetKey(window, GLFW_KEY_P);

        if ( lastOButtonState == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        {

            second = 0;
            minutes = 0;
            start_time = glfwGetTime();
            savedTime = 0;
        }
        lastOButtonState = glfwGetKey(window, GLFW_KEY_O);
    }

    // Shutdown GLFW
    glfwTerminate();
    return 0;
};



