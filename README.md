# Landstalker Engine (prototype)

This project aims to be a re-implementation of the DDS520 Engine used by Landstalker, ported to modern systems.

## Live browser demo
The web browser demo is [here](https://lordmir.github.io/landstalker_engine/). This reflects the current state of the repository.

## Rationale

After developing the [Landstalker Editor](https://github.com/lordmir/landstalker_editor) and working on the [Landstalker Disassembly](https://github.com/lordmir/landstalker_disasm), it became apparent that the technical limitations of building ROMs for the Sega Genesis / Megadrive were too significant to allow other users to build their own versions of the game. Additionally, the data format of the extracted maps (in their original form) are too complex to be easily editable.

## Project Aims

- Universal platform to develop and play Landstalker-inspired games for the PC.
- Options to enable quality-of-life UI improvements: object shadows, customisable control schemes, etc.
- To be as developer-friendly as possible, use text-based configuration files and standardised formats wherever possible.
-- Should support assets extracted from the ROM (e.g. palette-indexed bitmaps) as well as standard file formats (e.g. PNG).
-- Should support extensions and custom scripting, e.g. for new magic swords or items.
-- Should eventually support custom tooling / formats for map generation - so as to be a lot easier to create and modify maps versus the original highly-compressed format.
- Aim for 95% accuracy; engine code will be based off of the ROM disassembly. Should be able to run the original game without significant differences.
-- It should be possible to be extract scripts and assets from the original retail ROMs (using liblandstalker from the [Editor](https://github.com/lordmir/landstalker_editor) project), to be fully playable in the engine.  
- Shouldn't be constrained by the limitations of the orignal engine and platform. E.g. no palette restrictions on graphics, much higher entity limits, higher map counts and map sizes, etc.
-- Should have optional support for extended formats - e.g. heightmaps that support additional features such as ceilings / multiple levels, sloped floors, etc.

## Current status

- 2025/10/25 - Very basic proof-of-concept build using SDL3, demonstrating text output.

## Build instructions

- Build requires [CMake](https://cmake.org/) to be installed.
- You will also need a C++ compiler. On Linux, `gcc` is recommended. For Windows builds, you can either use the [Visual Studio Community compiler](https://visualstudio.microsoft.com/vs/community/), or [gcc](https://github.com/skeeto/w64devkit/releases).
- Run the following commands to build, from within the project directory:
```
cmake -S . -B build
cmake --build build --config Release
```
- The built executable should be located in the `build` directory. On Windows, you may need to look in the `build\Release` folder.

## Credits

- Kan Naito and the team at Climax Entertainment for making this incredible game!
