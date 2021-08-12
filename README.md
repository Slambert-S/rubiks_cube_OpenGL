# README #

# TEAM MEMBERS #
Antoine Dufour-Blain, 21308130, antoine.dufourblain@gmail.com
Kennedy Ballard, 40132850, kennedyjballard@gmail.com
Samuel Lambert-Senécal, 40112310, s_lambert-senecal@hotmail.fr
Ana Lucio, 40032548, catalan.lucio@hotmail.com
Thomas Rao, 40097357, thomas.rao@outlook.com
## General structure ##

* `doc` is for documentation, report, etc. Create a dir for your team there, e.g., `teamX`, and initialize the report document there.

## Templates ##

* `doc/report/project-report.tex` is the most documented example template in LaTeX, single column, more readable. To be able to compile LaTeX report, install and configure [MiKTeX](http://miktex.org) first (LaTeX backend compiler and styles and packages), then [TeXnicCenter](http://texniccenter.org) (GUI front-end to MikTeX, it will find MikTeX installation when installed after MikTeX). Open `project-report.tcp` project file for TeXnicCenter and compile it using F7 3 times; F5 to preview the PDF.

## Build Instructions

### Command Line

1. Download and Install CMake
2. Open a terminal
3. Run cmake:

```
cd <source_folder>
cmake -S . -B <build_folder>
cmake --build <build_folder> --target install
```

### CMake GUI

You can use the CMake GUI to configure and generate the Visual Studio Project.

1. Download and Install CMake
2. Open the CMake GUI
3. Set the `Where is the source code:` field to the source folder
4. Set the `Where to build the binaries:` field to different location (e.g.: source_folder/build)
5. Click Configure
6. Click Generate

#### Windows

Once that is created you can build the **ALL_BUILD** project followed by building
the **INSTALL** project.

#### Linux

In the terminal:

```
cd <build_folder>
make install
```


## Running

Once built and installed, all the relevant files will be in the **dist** folder
at the top level of the source folder.

### Linux/macOS

In the terminal type:

```
./comp371-w21-03
```

### Windows

Click on the **Project_Team_3.exe** from the File Explorer.

## References

Good place to learn OpenGL
https://www.learnopengl.com

Official OpenGL 4 Reference Documentation
https://www.khronos.org/registry/OpenGL-Refpages/gl4/

Official OpenGL Wiki
https://www.khronos.org/opengl/wiki/

Easy to Navigate OpenGL Function Documentation
http://docs.gl/

GLM Documentation
https://glm.g-truc.net/0.9.9/index.html

GLFW Documentation
https://www.glfw.org/

Good place for advanced graphics theory
https://www.scratchapixel.com/


##Texture Source:
All the following are copyright free
Metal: https://www.3dxo.com/textures/5331_iron_4
Box: https://www.3dxo.com/textures/531_beech_nature
Tiles: https://www.3dxo.com/textures/4965_wood_12
Stage: https://www.3dxo.com/textures/522_worn_cotton
All other textures made by us

## Features and Functionality
### Cameras
Model views:

- Use keys 1, 2, 3, 4, 5 to toggle between different cameras (one for each model). This also toggles which model is in focus for transformations.

Camera Movement:

 To remove if not needed

* Arrow up and arrow down keys shift the camera position in the y direction
* Arrow left and arrow right keys shift the camera position in the x direction
* Left mouse button and cursor movement in y direction zooms in (cursor up) and out(cursor down)
* Right mouse button and cursor movement in x direction pans camera left (cursor left) amd right (cursor right)
* Middle mouse button and cursor movement in y direction tilts camera up (cursor up) and doen (cursor down)

Model transformations:




* b to toggle shadows using the two-pass shadow algorithm 
* spacebar to re-position a model at a random location 


Timer Controll:
* Shift + P to start and pause the timer
* Shift + R to reset the timer

World Reset:


Added feature:

