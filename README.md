# CrabApplication

<img width="1282" height="992" alt="CrabApplication_fXLfYxzmar" src="https://github.com/user-attachments/assets/34bdea0c-e6e8-4956-b481-800e1fb10496" />

Crab Application is an application built with Crab Engine, to demonstrate usage, and to serve as a basic scene editor. It also makes use of the C# scripting features of Crab Engine.
#### Running the application with the flag --editor will open the editor. Otherwise it will run standalone. Alternatively the FORCE_EDITOR_ON define in main.cpp can be set to 1.

## CrabApplication Features
- Level editor
- Content browser
- Property inspector
- Scene running
- Asset hot reloading

## Build instructions
### Prerequisites
- .NET 9.0 Runtime Installed
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

3. Open Visual Studio Solution from the build folder
4. Build and Run CrabApplication (x64).
   The build may fail the first time as it sets up the C# dependencies. Run it again.

   
## Scripting
Changes can be made to C# script modules under CrabApplication/script. Scripts will be compiled upon building the application.
Changes can also be made at runtime. Edit the script, then run the "Build_\<Module\>_Scripts" target inside Visual Studio. The scripts will be recompiled and the DLL will be reloaded.
Native bindings are still work-in-progress.

## Known Issues
- Editor Jank
- Sometimes the C++ compilation can fail due to object mismatches. This is a modules bug, and building again will solve it.
- Runtime shader recompilation can be finnicky
