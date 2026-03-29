# MotdPFA
Welcome to MotdHS's mod of Piano From Above!

Not much has been done yet, but I am planning on experimenting with some stuff, maybe even trying to switch the graphics library because it's currently using the legacy DirectX 9 SDK.

This is based on the `viz` branch of Piano From Above, which just removes the "learning" features.

<sub>Please note that I am a beginner at C/C++ and don't know what I am doing at all (?)</sub>

## Differences from original PFA
- none yet

## To-Do
- Remove the protobuf dependency
- Switch graphics library
- Switch to CMake (probably)

## How to build
**NOTE**: Direct X SDK and Protobuf will no longer be needed in the future, but for now, they are required.

- Clone this repository
- Download and install Visual Studio Community 2026
  - If this version has issues, try the Insiders version.
- Download and install Direct X SDK
- Download and extract Google Protocol Buffers 2.5
  - In the `vsprojects` folder, run `extract_includes.bat`
  - Open `libprotobuf-lite.vcproj`
  - Add the x64 configuration if it doesn't exist
  - Build `libprotobuf-lite` (Release, x64)
- Download and extract raylib 5.5 source code
  - Open `projects\VS2022\raylib.sln`
  - Build `raylib` (Release.DLL, x64)
  - Copy `raylib.h`, `raymath.h`, and `rlgl.h` from `src` to `include` (make the directory if it doesn't exist)
- Open `PianoFromAbove.sln`
  - In VC++ Directories, edit Include Directories and Library Directories to point to your folders
  - In C/C++ -> General, edit Additional Include Directories to point to the `include` directory you just made for raylib
  - In Linker -> General, edit Additional Library Directories to point to your compiled raylib library
- Cross fingers
- Build! (Release, x64)
- Copy your compiled `raylib.dll` to the same folder the PFA executable is in
