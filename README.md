# MerlinEngine
Merlin is an engine for building desktop applications with 3D rendering. It is currently under construction and so it is sparse on features and largely unoptimized.

## Project Overview

Currently the project consists of a static library containing the engine, a sample demo app built using the engine, and a googletest test suite.

## Current Features
- Logging system
- Window application class with event handling
- ImGUI integration for building custom GUIs
- Scene object with extendable component system
- Input handling and access
- 3D rendering (in progress)
    - Perspective camera
    - Point, Spot and Directional lighting
    - Blinn-Phong shaders

## Getting Started
The core engine is a static library and is meant to be built into your final executable. It is recomended that you include the engine as a git submodule in to your own project for easiest use.

<ins>**1. Dependencies:**</ins>
The engine is written in C++17 and thus requires a compatible compiler.

Most of the projects dependencies are linked as submodules and should require no additional work. The only exception is OpenGL, which should come built in to most operating systems.

This project uses CMake as a build system. If you don't have it yet, you can get it [here](https://cmake.org/download/). 

<ins>**2. Download:**</ins>

Simply clone the repository or add it as a submodule to your project. Don't forget to add `--recursive` when cloning or run `git submodule update --init` to download the dependencies.

<ins>**3. Build:**</ins>

Instructions for using CMake can be found [here](https://cmake.org/runningcmake/).