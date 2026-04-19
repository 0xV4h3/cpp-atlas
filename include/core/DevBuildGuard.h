#pragma once

/**
 * @file DevBuildGuard.h
 * @brief Compile-time and runtime guards for developer-only features.
 *
 * CPPATLAS_DEV_BUILD is defined by CMake only in Debug / RelWithDebInfo builds
 * or when -DCPPATLAS_DEV_BUILD=ON is explicitly passed at configure time.
 *
 * All admin features (admin panel, quiz_admin CLI, admin account creation)
 * are gated behind this macro.  In a Release build the macro is absent and
 * every guarded code path is dead-eliminated by the compiler.
 *
 * Usage in .cpp / .h files:
 *
 *   #include "core/DevBuildGuard.h"
 *
 *   #ifdef CPPATLAS_DEV_BUILD
 *   // developer-only code
 *   #endif
 *
 *   // or use the inline helper:
 *   if (DevBuild::enabled()) { ... }   // branch optimised away in Release
 */

namespace DevBuild {

/// Returns true only in dev builds (Debug / explicit -DCPPATLAS_DEV_BUILD=ON).
/// In Release this is a constexpr false and the branch is dead-code-eliminated.
#ifdef CPPATLAS_DEV_BUILD
    inline constexpr bool enabled() noexcept { return true; }
#else
    inline constexpr bool enabled() noexcept { return false; }
#endif

} // namespace DevBuild
