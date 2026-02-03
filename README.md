# CppAtlas

Educational Qt-based offline environment for learning, visualizing, and benchmarking modern C++. Modular architecture integrates code transformation (cppinsights), assembly analysis (Compiler Explorer), benchmarking (Google Benchmark, Quick Bench), and quiz modules for students and educators.

## Features

- **CppInsights integration:** View transformed C++ code side-by-side, with syntax highlighting and educational commentary.
- **Assembly view:** Compile code, analyze and compare source/assembly, mapping lines as in Compiler Explorer.
- **Benchmarking:** Run and visualize local benchmarks using Google Benchmark, Quick Bench, and Build Bench tools.
- **Quiz module:** Interactive tasks testing C++ concepts; scoring, history, randomization.
- **Offline-first:** All tools, compilers, and data operate entirely locally (no internet required).

## Project Structure

```
/src        # Source code (core, modules: cppinsights, asm, benchmark, quiz)
/include    # Public headers, module interfaces
/ui         # Qt UI files (.ui)
/tools      # Bundled binaries (insights, compilers, benchmark)
/tests      # Unit/integration tests; educational scenarios
/resources  # Icons, localization, docs
```

## Build Instructions

1. Install Qt 6+ and CMake ≥ 3.16.
2. Clone the repository.
3. Open CMakeLists.txt in Qt Creator, configure the kit, build, and run.
4. Place compatible tools/binaries in `/tools` as needed.

## License

MIT License (see LICENSE file for details)

## References

- [C++ Insights](https://github.com/andreasfertig/cppinsights)
- [Compiler Explorer](https://github.com/compiler-explorer/compiler-explorer)
- [Google Benchmark](https://github.com/google/benchmark)

## Phase 1: IDE Core - Build & Run

### Prerequisites

- Qt 6.4+ (Qt5 also supported)
- CMake 3.16+
- C++17 compatible compiler (GCC 13+ or Clang 17+)

### Ubuntu/Debian Installation

```bash
sudo apt-get install qt6-base-dev qt6-base-dev-tools cmake g++
```

### Building

```bash
git clone https://github.com/0xV4h3/cpp-atlas.git
cd cpp-atlas
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Running

```bash
./src/CppAtlas
```

### Testing

```bash
cd build
./tests/CppAtlasTests
# Or with offscreen platform:
QT_QPA_PLATFORM=offscreen ./tests/CppAtlasTests
```

### Features Implemented (Phase 1)

- ✅ Multi-file code editor with C++ syntax highlighting
- ✅ Compiler abstraction (GCC & Clang support)
- ✅ Build & Run integration
- ✅ Problems panel with diagnostic parsing
- ✅ File tree navigation
- ✅ Output panels (Build, Run, Problems)
- ✅ Auto-detection of system compilers
- ✅ Configurable C++ standards (C++11 through C++23)

### Usage

1. **Create/Open Files**: Use File → New or File → Open
2. **Select Compiler**: Choose from toolbar dropdown
3. **Build**: Press F7 or Build → Build
4. **Run**: Press F5 or Build → Run
5. **View Errors**: Check Problems tab for compiler diagnostics

