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
