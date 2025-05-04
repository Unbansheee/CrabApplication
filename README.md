# CrabApplication

CrabApplication is an application built with CrabEngine
#### Running the application with the flag --editor will open the editor. Otherwise it will run standalone. Alternatively the FORCE_EDITOR_ON define in main.cpp can be set to 1.

## CrabApplication Features
- Level editor
- Content browser
- Property inspector
- Scene running
- Asset hot reloading

## CrabEngine Features
- WebGPU-Native 3D renderer
- Slang shader compilation
- Material system based on Slang modules
- .NET Core C# scripting
- .NET DLL hot reloading
- Jolt physics
- Custom reflection rystem
- Automatic property serialization and deserialization
- C++20 Modules
  
## Build instructions
### Prerequisites
- .NET 9
- MSVC 17.6 (others untested. Compiler supporting C++20 modules and import std is required)
- CMake 3.28 or newer (others untested)

### Build Instructions
1. Clone the repository (with submodules):
```
git clone https://github.com/Unbansheee/CrabApplication.git --recursive
cd CrabApplication
```

2. Generate Project Files
#### Configuration
- DEV_MODE: 
-- ON: When this option is turned on in CMake, the application will read resources from the source directory
-- OFF: When this option is turned off in CMake, the application will read resources from the compiled exe directory
  
```
cmake -S . -B build -G "Visual Studio 17 2022" -DDEV_MODE:BOOL=OFF
```

3. Open Visual Studio Solution
4. Build and Run CrabApplication (x64).
   The build may fail the first time as it sets up the C# dependencies. Run it again.

   
## Scripting
Changes can be made to C# script modules under CrabApplication/script. Scripts will be compiled upon building the application.
Changes can also be made at runtime. Edit the script, then run the "Build_\<Module\>_Scripts" target inside Visual Studio. The scripts will be recompiled and the DLL will be reloaded.
Native bindings are still work-in-progress.
