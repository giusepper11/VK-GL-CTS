# Horizon Vulkan CTS

This headless libnx port links a loaderless `libvulkan.a` from
`giusepper11/mesa-switch`. It uses the standard CTS application, shader
compilation, test implementations, result handling, and QPA logging.
The driver is an external, read-only build input.

## Build

Install devkitPro's devkitA64, libnx, Switch tools and portlibs, including
libpng, zlib, expat and zstd. Use a driver archive built without the optional
libelf-dependent `VK_NVX_binary_import` support. Export `DEVKITPRO` and fetch the
CTS-pinned sources:

```sh
python3 external/fetch_sources.py --protocol https
cmake -S . -B build-horizon -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE="$PWD/targets/horizon/toolchain.cmake" \
  -DDEQP_TARGET=horizon -DCMAKE_BUILD_TYPE=Release \
  -DDEQP_HORIZON_VULKAN_LIBRARY=/absolute/path/to/mesa-switch/libvulkan.a
cmake --build build-horizon --target deqp-vk_nro -j4
```

The SDK's `nx_create_nro` packages `deqp-vk.nro` beside the linked executable
under `build-horizon/external/vulkancts/modules/vulkan/`.
The complete Vulkan package is built; `depth-only.txt` selects only the two
original monolithic separate-render-pass cases, without `add_view_index`.

## Later hardware test

Use a homebrew environment with full application memory. Copy the NRO,
`targets/horizon/depth-only.txt`, and the `external/vulkancts/data/vulkan`
resource directory to an SD-card directory such as `sdmc:/switch/cts/`.
Launch with these arguments using a launcher that supports homebrew argv:

```text
--deqp-caselist-file=sdmc:/switch/cts/depth-only.txt
--deqp-archive-dir=sdmc:/switch/cts
--deqp-log-filename=sdmc:/switch/cts/depth-only.qpa
```

Keep the resource directory named `vulkan` beneath the archive directory.
The two selected cases generate shaders in CTS and do not require a
replacement shader cache or custom runner. Inspect the QPA for both case
results and the completion summary; successful packaging does not establish
a hardware pass.

## Scope and limitations

There is no WSI, GL/EGL backend, dynamic library loading, or subprocess
support. An explicit `--deqp-vk-library-path` is rejected because Vulkan is
linked statically. Vulkan SC, executor/execserver, and other test groups are
outside this initial port's validation scope. The existing CTS build policy
disables Vulkan video on unsupported operating systems, including Horizon.
Clocks and threading use libnx's POSIX support and CTS's existing thread-based
timer fallback. Crash recovery is unavailable on this platform; a fatal
assertion aborts rather than fabricating a test result.
Core-count queries use the existing conservative single-core fallback because
this SDK does not implement `sysconf`.

Do not infer driver correctness, launcher compatibility, runtime memory
usage, or on-device results from host checks and cross-build validation.
