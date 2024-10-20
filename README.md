![Ubuntu](https://github.com/Tolc-Software/frontend.wasm/workflows/Ubuntu/badge.svg) ![MacOS](https://github.com/Tolc-Software/frontend.wasm/workflows/MacOS/badge.svg) ![Windows](https://github.com/Tolc-Software/frontend.wasm/actions/workflows/windows.yml/badge.svg) ![Windows-debug-deploy](https://github.com/Tolc-Software/frontend.wasm/actions/workflows/windows-debug-deploy.yml/badge.svg)

# frontend.wasm #

`frontend.wasm` is a library that takes an AST defined by [`IR::IR`](https://github.com/Tolc-Software/IntermediateRepresentation) and writes [`embind`](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html) code. The idea is to be able to use the C++ code that corresponds to the AST from the web via the generated `embind`.

`frontend.wasm` is a part of the [`Tolc`](https://github.com/Tolc-Software/tolc) project and is used to support creating `WebAssembly` bindings.

## Building ##

Here is a general overview:

Requirements:

* CMake
* Emscripten (only for tests)
* conan
* clang
* For the tests:
  * [Emscripten](https://emscripten.org/docs/getting_started/downloads.html) (Compile integration tests)
  * [npm & node](https://nodejs.org/en/download/) (Used to run parts of integration tests with [Jest](https://jestjs.io/))

Configure the project:

```shell
$ cmake -S. -Bbuild -G Ninja -DCMAKE_CXX_COMPILER="clang++" -DCMAKE_C_COMPILER="clang" -DCMAKE_BUILD_TYPE="Debug" -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=cmake/conan_provider.cmake -DENABLE_TESTING=ON -DENABLE_PACKAGING=ON
```

Build `tolc`:

```shell
$ cmake --build build
```

Test with `ctest`:

```shell
$ cd build
$ ctest --output-on-failure
```

Install with `CPack`:

```shell
$ cpack -G TGZ --config build/CPackConfig.cmake
```
