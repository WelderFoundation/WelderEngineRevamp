# Welder Engine

The Welder Engine is a cross platform game engine that can be used to make simulations of any kind. Projects range from simple prototypes to professional 2D and 3D games, simple physics or graphics demonstrations to robust, realistic simulations.

![Editor](https://github.com/WelderFoundation/Media/blob/main/Editor/Editor-MineCartLevel2_2.png?raw=true)

# Features

## Editor
- A discrete launcher that provides access to all project and engine builds
- Editor modes customized for 2D and 3D projects
- Full-featured text editor with dozens of hotkeys and configuration options 

## Architecture
Component-based architecture applied across the engine in the form of :
- ZilchComponents for game object behaviors 
- ZilchFragment for fragment, vertex, and geometry shaders
- ContentComponents for Resources meta data 

## Graphics
- Fully modifiable, scripted rendering pipeline
- With a physically-based renderer provided as the default renderer
- Dedicated render thread

## Physics
- Custom-built, three-dimensional, constraint-based physics engine
- Regions that can apply both pre-defined and user-defined PhysicsEffects
- Robust, customizable joint system

## Audio
- Scripted, node-based DST system
- 3D sound positioning and attenuation

## Zilch
- High-level scripting and shader language

# Building

Building on Windows:
- Install CMake and Node.js
- Run the following in cmd:

```shell
git clone https://github.com/WelderFoundation/WelderEngineRevamp.git
cd WelderEngineRevamp
git submodule update --init --recursive
npm install

# Specify builder if not using the default. Ex: --builder="Visual Studio 17 2022"
node index.js cmake

.\Build\<target>\Welder.sln
```
