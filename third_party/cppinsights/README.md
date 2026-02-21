# CppInsights Prebuilt Binary

Place the `insights` (Linux/macOS) or `insights.exe` (Windows) binary in `third_party/cppinsights/bin/`.

## Download

- GitHub Releases: https://github.com/andreasfertig/cppinsights/releases
- Or install via package manager and configure the path in Tools > Settings > Tools

## Why prebuilt?

CppInsights depends on LLVM/Clang headers and cannot be compiled as part of the CppAtlas
build without a full LLVM toolchain. A prebuilt binary is the practical approach for
distribution. See: https://github.com/andreasfertig/cppinsights#installation

## Auto-detection order

`ToolsConfig::autoDetectCppInsights()` searches:
1. `third_party/cppinsights/bin/insights[.exe]` (this directory)
2. System PATH (`which insights` / `where insights.exe`)
3. Common install locations (`/usr/local/bin`, `/opt/homebrew/bin`, etc.)

Configure manually via `resources/config/tools.json` or the future Tools > Settings dialog.
