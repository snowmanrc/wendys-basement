# Wendy's Basement

A `horror game` I made along with my fellows for my `Programming Fundamentals` semester project.

## Premise

+ You are stuck in *Wendy's basement*.
+ To win, you must collect *5 crosses* randomly scattered across the basement.
+ Wendy lurks around the basement and will start *chasing* once she spots you.
+ To save yourself from her, you must *hide* in one of the crates in the basement.

## Controls

+ `W` Move up
+ `A` Move left
+ `S` Move down
+ `D` Move right
+ `Shift` To run
+ `E` To hide
+ `1,2,3` To choose menu option
+ `Enter` To selecte menu option

## Project Structure

The project has been divided cleanly into several components with their defined functionalities.
```
src/ -> Main Code
    main     -> Entry point
    game     -> Main game loop & Core logic
    player   -> Functionality related to the player
    ghost    -> Functionality related to the ghost/wendy
    map      -> Map layout and init
    sound    -> Sound controlling
    graphics -> Complex rendering

assets/ -> Game Assets
```

## Project Features (for evaluation)

+ Graphics library `sfml` used, specifically 3.x version.
+ Pathfinding algorithm used is `A*`
+ Simplistic `Ghost AI` (chasing and wandering)
+ Contains a `dynamic map loader` to load the map from a png.
+ Has `switchable debugging` for development vs production builds.
+ Game Mechanics:
    + Sprinting with Stamina
    + Hiding
    + Cross Collection
    + Collisions and Player Movement
    + Ghost Pathfinding and Movement
    + Ghost Wandering and Chasing Logic


## Installation

### Dependencies & Pre-Building

+ C++ Version Required: `C++ 20`
+ CMake Version Required: `CMake 3.22+`
+ SFML: `3.x` *essential*
+ Working audio and graphics driver and OpenGL support.

**Windows:** Download SFML 3 (e.g. "Visual C++ 17 (2022) - 64-bit") and unpack it to
`C:\SFML`, so that `C:\SFML\lib\cmake\SFML` exists. If it lives elsewhere, add
`-DCMAKE_PREFIX_PATH="path\to\SFML"` to the first CMake command below. The required
DLLs and the `assets/` folder are copied next to the executable automatically.

**Linux/macOS:** SFML 3 must be built from source or installed to a custom prefix.
See https://www.sfml-dev.org/download/ and pass `-DCMAKE_PREFIX_PATH=<install path>`.

### Cloning

Create a local clone of the repo using the following command:
```bash
git clone --depth 1 https://github.com/snowmanrc/wendys-basement.git
```
or using `ssh`:
```bash
git clone --depth 1 git@github.com:snowmanrc/wendys-basement.git
```

### Building

Open the directory in a terminal and run the command:
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

### Running

**Windows (MSVC):**
```bash
build\Release\WendysBasement.exe
```

**Linux/macOS:**
```bash
./build/WendysBasement
```

## Assets

+ Music from Youtube.
    + `Old Doll` and `Old Doll Slowed`
    + `Faint Signal`
    + `Giorno's Theme - Orchesta - il vento doro`
+ Sound Effects
    + From free sound websites
+ Art
    + Most of it is hand made by me :P
+ Font
    + Silkscreen

## Credits

+ Abdul Haseeb
+ Abdul Raheem Irfan
+ Hamza Shakeel
