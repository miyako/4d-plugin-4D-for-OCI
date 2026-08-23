# Oracle Instant Client Setup

This directory holds the Oracle Instant Client libraries and SDK headers.
They are **not committed to the repository** due to Oracle's OTN license restrictions.

## Download

1. Go to [Oracle Instant Client Downloads](https://www.oracle.com/database/technologies/instant-client/downloads.html)
2. Download the following packages for each target platform:

### macOS (Apple Silicon / aarch64)

- **Instant Client Basic** → extract to `oracle/macos-aarm64/`
- **Instant Client SDK** → extract SDK headers to `oracle/include/`

### Windows (x64)

- **Instant Client Basic** → extract to `oracle/windows-x64/`
- **Instant Client SDK** → extract SDK headers to `oracle/include/` (same headers for both platforms)

## Expected Layout

```
oracle/
├── README.md          (this file)
├── include/           (OCI SDK headers — oci.h, ociap.h, oratypes.h, etc.)
├── macos-aarm64/      (libclntsh.dylib, libclntshcore.dylib, libnnz.dylib, etc.)
└── windows-x64/       (oci.dll, oci.lib, orannz.dll, orasql.dll, etc.)
```

## CI/CD

The GitHub Actions workflow downloads these packages automatically at build time.
