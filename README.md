# Sudoku

This is a [Sudoku] implementation in [C++] using minimum amount of dependencies. It is not complete
yet, but it will be...

## Building

### Linux

```sh
$ premake5 gmake2
$ make
```

### Windows

**Note:** Windows support is not tested.

- For `MSVC`

```sh
$ premake5 vs2022
$ msbuild Sudoku.sln
```

- For `MinGW`

```sh
$ premake5 gmake2
$ make
```

## Dependencies

- [GLFW]
- [OpenGL]

[Sudoku]: https://www.wikiwand.com/en/Sudoku
[C++]: https://www.wikiwand.com/en/C++
[GLFW]: https://www.glfw.org/
[OpenGL]: https://www.opengl.org/
