# Amazing Game
A game like asteroids written in C++
*All Music and Code Written by me*

**There is no release for MacOS unfortunatly, building will work though**

## Building
1. `git clone https://github.com/bit-turtle/game-app`
   * On Windows use the Git Command Prompt
2. `cd game-app`
   * On Windows use the Visual Studio Command Prompt
2. `cmake .`
   * On Windows use the Visual Studio Command Prompt
3. `make` or `msbuild PACKAGE.vcxproj -p:Configuration=Release`
   * On Windows use the Visual Studio Command Prompt
4. `./bundle.sh` or `bundle.bat` to get important files in a folder called `bundle`

## How to Open The Terminal
* Linux, usually [Ctrl+Alt+T] but you already knew that.
* Windows, use the start menu to search for the terminal/command prompt you need
* MacOS, in Finder, open the `Applications/Utilities` folder and double-click on `Terminal`

## Things Required To Build
* All Platforms:
  1. Git
     * On Linux, your package manager will have it
     * On Windows and MacOS, download from [git-scm.com](https://git-scm.com/)
  2. CMake
     * On Linux, your package manager will have it
     * On Windows and MacOS, download from [cmake.org](https://cmake.org/)
* Linux
  1. Make
     * Your package manager will have it
* Windows
  1. Visual Studio C++ Command Line Tools
     * Download from [visualstudio.microsoft.com](https://visualstudio.microsoft.com/visual-cpp-build-tools/)
* MacOS
  1. XCode Command Line Tools
     * Install XCode
     * Run `xcode-select --install`
