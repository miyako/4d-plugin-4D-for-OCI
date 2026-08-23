# 4D for OCI

A 4D plugin that provides Oracle database connectivity via the Oracle Call Interface (OCI).
This is a modern clone of the original "4D for OCI" plugin (deprecated 2020), rebuilt for
64-bit macOS (Apple Silicon) and Windows (x64).

## Status

**Phase 1** — Core subset (42 commands):

| Theme | Commands | Status |
|-------|----------|--------|
| Connection | `OCIEnvCreate`, `OCIHandleAlloc/Free`, `OCILogon/off`, `OCIServerAttach/Detach`, `OCISessionBegin/End`, `OCIBreak`, `OCIReset`, `OCIParamGet/Set`, `OCIPasswordChange` | ✅ Implemented |
| Datatype | `OCIStmtPrepare/Execute/Fetch`, `OCIBind/DefineByPos/Name`, `OCIAttrGet/Set`, `OCIErrorGet`, `OCIServerVersion`, `OCIDescriptor*`, `OCIDescribeAny*` | ✅ Implemented (Bind/Define stubbed) |
| Transaction | `OCITransStart/Commit/Rollback/Prepare/Detach/Forget` | ✅ Implemented |
| Extras | `OCITerminate`, `OCIOnErrCall`, `OCIGetTnsnamesPath`, `OCISetEnv`, `OCIGetEnv` | ✅ Implemented |

## Prerequisites

- **4D v21+** (64-bit)
- **Oracle Instant Client** (Basic + SDK) — see [oracle/README.md](oracle/README.md)
- **CMake 3.20+**
- **Xcode** (macOS) or **Visual Studio 2022** (Windows)

## Building

```bash
# Clone with submodules
git clone --recurse-submodules https://github.com/miyako/4d-plugin-4D-for-OCI.git
cd 4d-plugin-4D-for-OCI

# Set up Oracle Instant Client (see oracle/README.md)
# ...

# Build (macOS)
cd 4D-for-OCI
cmake -B cmake-build -G Xcode
cmake --build cmake-build --config Release

# Build (Windows)
cd 4D-for-OCI
cmake -B cmake-build -G "Visual Studio 17 2022" -A x64
cmake --build cmake-build --config Release
```

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
4D for OCI clone
