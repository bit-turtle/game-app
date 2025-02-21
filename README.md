# Amazing Game
A game like asteroids written in C++
*All Music and Code Written by me*

Releases have been made for the following platforms:
* Windows (x86_64)
* Linux (x86_64)
* Linux (armv8)
**There is no release for MacOS unfortunatly, building the game will work on MacOS though**

## Things Required To Build
* All Platforms:
  1. Git
     * On Linux, your package manager will have it (`apt install git` or `pacman -S git`)
     * On Windows and MacOS, download from [git-scm.com](https://git-scm.com/)
  2. CMake
     * On Linux, your package manager will have it (`apt install cmake` or `pacman -S cmake`)
     * On Windows and MacOS, download from [cmake.org](https://cmake.org/)
* Linux
  1. Make and g++
     * Your package manager will have it (`apt install buildessential` or `pacman -S base-devel`)
* Windows
  1. Visual Studio C++ Command Line Tools
     * Download from [visualstudio.microsoft.com](https://visualstudio.microsoft.com/visual-cpp-build-tools/)
* MacOS
  1. XCode Command Line Tools
     * Install XCode
     * Run `xcode-select --install`

## How to Open The Terminal
* Linux, usually [Ctrl+Alt+T] but you probably already know how.
* Windows, use the start menu to search for "Developer Powershell for VS 2022"
* MacOS, in Finder, open the `Applications/Utilities` folder and double-click on `Terminal`

## Building
1. `git clone https://github.com/bit-turtle/game-app` to download the game code
2. `cd game-app` to enter the game directory
3. `mkdir build` to create the build directory
4. `cmake build` to create the build files in the build directory
5. `cmake --build build --config Release -j 4` to build the game ("4" can be replaced with the number of cores on your computer)
   * On Windows use the Visual Studio Command Prompt
6. `./bundle.sh` or `.\bundle.bat` to get all important files in a folder called `bundle`
7. Open `Asteroidal` or `Asteroidal.exe` to play the game!
