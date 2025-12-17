# cpp-chat

A simple terminal chat application

## Running the application

Install `vcpkg` set `export VCPKG_ROOT="$HOME/vcpkg"` to your `~/.zshrc` or equivalent (PATH variables/global environment variables).

## Dependencies

- cmake (similar to vite)
- vcpkg (similar to npm)
- ncurses
- fmt

To handle terminal input and rendering we're using C++11 threads feature as well as `ncurses`. One to listen for input, and one to render to the screen.

### ncurses

I've also added `ncurses` requirement to the `CMakeLists.txt` to facilitate how terminal is handled. Without that I noticed that I was going into a dark path of copy pasting code that I had no idea what were doing. For more information please visit:

- https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/intro.html#WHATIS
- https://cmake.org/cmake/help/latest/module/FindCurses.html

### fmt

`fmt` is being used to facilited concatenating strings in an easy way. In C++20 `std::format` was introduced. But I prefer to use older things as I assume these are more commonly used in the job market.

## Learning Resources

- NCURSES Programming HOWTO: https://tldp.org/HOWTO/pdf/NCURSES-Programming-HOWTO.pdf
- Beej’s Guide to Network Programming:
  https://beej.us/guide/bgnet
