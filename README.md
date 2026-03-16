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

## Emoji Support (Linux/WSL)

To see color emoji icons in quiz topics/UI, install:

    sudo apt install fonts-noto-color-emoji

If not installed, some emojis (e.g. in quiz topics or navigation) will show as empty squares or boxes.

---

### Administrator Tools

CppAtlas includes a built-in admin panel for managing quiz content.

#### Entry points

| Method | Description |
|---|---|
| `--admin` CLI flag | `./CppAtlas --admin` — opens the admin panel immediately after login |
| `Ctrl+Alt+Shift+M` | Keyboard shortcut while the main window is active |

#### Release password gate

In **release builds**, the admin panel requires a second confirmation step.  
Set the `CPPATLAS_ADMIN_HASH` environment variable to the **SHA-256 hex digest** of your
admin password before launching:

```bash
export CPPATLAS_ADMIN_HASH="$(echo -n 'YourPassword' | sha256sum | cut -d' ' -f1)"
./CppAtlas
```

In **debug builds** (`QT_DEBUG`) the second password prompt is suppressed for convenience.

#### CLI tool (`quiz_admin`)

```bash
# Show database statistics
quiz_admin --db /path/to/cppatlas.db stats

# Validate content integrity + patch status
quiz_admin --db /path/to/cppatlas.db validate --content-dir ./patches

# Apply pending SQL patches
quiz_admin --db /path/to/cppatlas.db apply-content --content-dir ./patches

# Export content tables to a SQL dump
quiz_admin --db /path/to/cppatlas.db export --out backup.sql
```

See [`tools/quiz_admin/README.md`](tools/quiz_admin/README.md) and
[`docs/admin_workflow.md`](docs/admin_workflow.md) for the full workflow.

## License

MIT License (see LICENSE file for details)
