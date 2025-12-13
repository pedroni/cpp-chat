# cpp-chat

A simple terminal chat application

## How does it work?

To handle terminal input and rendering we're using C++11 threads feature as well as `ncurses`. One to listen for input, and one to render to the screen.

I've also added `ncurses` requirement to the `CMakeLists.txt` to facilitate how terminal is handled. Without that I noticed that I was going into a dark path of copy pasting code that I had no idea what were doing. For more information please visit:

- https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/intro.html#WHATIS
- https://cmake.org/cmake/help/latest/module/FindCurses.html

## CMake

### vcpkg

This project is using `vcpkg` to properly use it, you have to install it and set `export VCPKG_ROOT="$HOME/vcpkg"` to your `~/.zshrc` or equivalent (PATH variables/global environment variables).
