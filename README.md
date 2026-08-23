# 4D for OCI

A 4D plugin that provides Oracle database connectivity via the Oracle Call Interface (OCI).
This is a modern clone of the original "4D for OCI" plugin (deprecated 2020), rebuilt for
64-bit macOS (Apple Silicon) and Windows (x64).

## Status

**Phase 1 complete** — 42 commands, all implemented:

| Theme | Commands | Count |
|-------|----------|-------|
| Connection | `OCIEnvCreate`, `OCIHandleAlloc/Free`, `OCILogon/off`, `OCIServerAttach/Detach`, `OCISessionBegin/End`, `OCIBreak`, `OCIReset`, `OCIParamGet/Set`, `OCIPasswordChange` | 14 |
| Datatype | `OCIStmtPrepare/Execute/Fetch`, `OCIBindByPos/Name`, `OCIDefineByPos`, `OCIStmtGetBindInfo`, `OCIAttrGet/Set`, `OCIErrorGet`, `OCIServerVersion`, `OCIDescriptor*`, `OCIDescribeAny*` | 17 |
| Transaction | `OCITransStart/Commit/Rollback/Prepare/Detach/Forget` | 6 |
| Extras | `OCITerminate`, `OCIOnErrCall`, `OCIGetTnsnamesPath`, `OCISetEnv`, `OCIGetEnv` | 5 |

## Download

Signed and notarized builds are available on the [Releases](https://github.com/miyako/4d-plugin-4D-for-OCI/releases) page. Each release includes a `.zip` (cross-platform bundle with macOS + Windows binaries) and a `.dmg` (macOS only, stapled).

## Prerequisites

- **4D v21+** (64-bit)
- **Oracle Instant Client** (Basic + SDK) — see [oracle/README.md](oracle/README.md)
- **CMake 3.20+**
- **Xcode Command Line Tools** (macOS) or **MSVC** (Windows)

## Building

```bash
# Clone with submodules
git clone --recurse-submodules https://github.com/miyako/4d-plugin-4D-for-OCI.git
cd 4d-plugin-4D-for-OCI

# Set up Oracle Instant Client (see oracle/README.md)
# Copy dylibs to oracle/macos-aarm64/ or DLLs to oracle/windows-x64/

# Build (macOS)
cd 4D-for-OCI
cmake -B cmake-build -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build --config Release

# Build (Windows — requires MSVC in PATH)
cd 4D-for-OCI
cmake -B cmake-build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build --config Release
```

The built plugin is placed in `4D-for-OCI-test/Plugins/`.

## Testing

Test methods are in `4D-for-OCI/4D-for-OCI-test/Project/Sources/Methods/`. Run `test_all` in 4D to execute all 9 test suites (extras, handles, error, connection, attr, stmt, transaction, describe, server).

## CI/CD

- **`build.yml`** — compiles and verifies both platforms on every push/PR
- **`release.yml`** — builds, codesigns (Developer ID), notarizes, and publishes a GitHub Release on `v*.*.*` tags or manual dispatch

Oracle Instant Client binaries are stored as GitHub Release assets (tag `oracle-libs`) and downloaded automatically by CI.

## Architecture

OCI handles (C pointers) are stored in an internal handle table and exposed to 4D as
integer IDs (`Longint`). The plugin converts between UTF-16 (4D strings) and UTF-8
(Oracle strings) automatically.

### Key differences from the original plugin

| Original | Clone |
|----------|-------|
| `&S` (C_STRING) parameters | `&T` (Text) for input, `&Z` (Pointer) for output |
| `&8` (double) parameters | `&R` (Real) |
| 32-bit pointer-as-longint | Handle table (64-bit safe) |

## License

MIT (plugin code). Oracle Instant Client is subject to the [OTN License](https://www.oracle.com/downloads/licenses/instant-client-lic.html).
