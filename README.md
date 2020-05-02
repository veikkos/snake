# Snake Game (Game Boy Advance, SDL)

Simple multiplatform Snake game originally written for PC using SDL
years ago as a personal game development studying. Since then it was
ported for Game Boy Advance.

![Screenshot](https://github.com/veikkos/snake/blob/master/media/image.jpg)

## Compiling

### Debian-like systems

Make sure you have C++ compiler and cmake installed.

Install SDL2 and libraries
```
$ sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

Build
```
$ cmake .
$ cmake --build .
```

### Visual Studio

SDL2 library can be downloaded from https://www.libsdl.org/.

You might need to specify SDL2 library paths explicitly, e.g.

```
$ cmake -G "Visual Studio 15 2017" -DSDL2_PATH="C:\\<path>\\SDL2-2.0.9-VC" -DSDL2_IMAGE_PATH="C:\\<path>\\SDL2_image-2.0.4-VC" -DSDL2_TTF_PATH="C:\\<path>\\SDL2_ttf-2.0.14-VC" .
```

### Game Boy Advance

Needs devkitPro in path.

```
$ mkdir gb-build
$ cd gb-build
$ cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../toolchains/gba.cmake ..
$ cmake --build .

```
