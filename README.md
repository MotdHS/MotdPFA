# MotdPFA
Welcome to MotdHS's mod of Piano From Above!

Not much has been done yet, but I am planning on experimenting with some stuff.

This is based on the `viz` branch of Piano From Above, which just removes the "learning" features.

<sub>Please note that I am a beginner at C/C++ and don't know what I am doing at all (?)</sub>

## Differences from original PFA
- Switched from DirectX 9 to raylib (OpenGL)
- Removed library panel (no more protobuf dependency :D)
- Windows ARM64 compatibility *(Untested, doesn't work on my Snapdragon 7c Gen 2 that only supports OpenGL ES)*

## To-Do
- Cleanup (probably messy af right now :D)
- Try to switch to Qt or GTK
- Switch to RtMidi for MIDI output
- Get rid of `Windows.h`
- Switch to Clang
- Switch to CMake
- Linux compatibility?

## How to build

- Clone this repository
- Download and install Visual Studio Community 2026
  - If this version has issues, try the Insiders version.
- Download and extract raylib 5.5 source code
  - Open `projects\VS2022\raylib.sln`
  - Build `raylib` (Release.DLL, x64)
  - Copy `raylib.h`, `raymath.h`, and `rlgl.h` from `src` to `include` (make the directory if it doesn't exist)
- Open `PianoFromAbove.sln`
  - In C/C++ -> General, edit Additional Include Directories to point to the `include` directory you just made for raylib
  - In Linker -> General, edit Additional Library Directories to point to your compiled raylib library
- Cross fingers
- Build! (Release, x64)
- Copy your compiled `raylib.dll` to the same folder the PFA executable is in
