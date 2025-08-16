# WIP: Visage + CLAP Plugin CMake Template
Minimal [CLAP](https://github.com/free-audio/clap) example implementing a gain knob with a [visage GUI](https://github.com/VitalAudio/visage).

Since this is work in progress, expect:
- Changes in code locations, things are likely not to stay as they are right now.
- Clunky and incomplete UI, focus is mostly on code structure for now, not UI/UX.

On the one hand, this project should serve as a starter for various plugins, so I try to implement some generalized concepts.
On the other hand, this project is also meant play with some architecture concepts, so the implementations may seem bloated and overkill.


## Build and Tests
### Out-of-source Build on Linux

```bash
# Assume parent directory of git repository clone
mkdir build && cd build

# E.g. build with ninja
cmake -G "Ninja" ../Clap-Visage-Template.git
# Or choose a compiler
CC=clang CXX=clang++ cmake -G "Ninja" -DCMAKE_BUILD_TYPE=RelWithDebInfo path/to/source
CC=gcc CXX=g++ cmake -G "Ninja" -DCMAKE_BUILD_TYPE=RelWithDebInfo path/to/source

cmake --build .
```


### Build with Sanitizers

```bash
# Assume parent directory of git repository clone
mkdir build_address && cd build_address
CC=clang CXX=clang++ cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-fsanitize=address -fsanitize=undefined" path/to/source
cd .. && mkdir build_cmemory && cd build_memory
CC=clang CXX=clang++ make -G "Ninja" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-fsanitize=memory" path/to/source
cd .. && mkdir build_thread && cd build_thread
CC=clang CXX=clang++ cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-fsanitize=thread" path/to/source
```


### Run Tests on Linux

Tests implemented with [doctest](https://github.com/doctest/doctest)

```bash
ctest --output-on-failure
```


### Run Code Coverage Test in Debug Builds

- Requires `gcov`, `lcov` and `genhtml` to be available
- Build it with gcc instead of clang
- Set `COMPILE_WITH_COVERAGE` to `ON`

```bash
# Build with gcc
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Debug -DCOMPILE_WITH_COVERAGE=ON ../cppcmaketemplate
cmake --build .
ninja coverage
# Open the report in your favorite browser
firefox coverage/index.html

# or you could do
ctest -T covearge
```


### VS Code/Codium Dev Setup

To enable tests, clang-tidy checks, etc.

- Install external tools separately: clang, clang-tidy, cmake, CodeChecker, lizard
- Install extensions: CodeChecker, CMake, C++
- Set the compile_command path to: `${workspaceFolder}/build/compile_commands.json`
- Set argument to: `--ignore ${workspaceFolder}/.skipfile --analyzer-config clangsa:mode=shallow --ctu --tidy-config ${workspaceFolder}/.clang-tidy`
