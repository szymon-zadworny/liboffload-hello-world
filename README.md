# liboffload hello world

This repository contains a liboffload SPIR-V kernel launch example written in C. This example only
works on Level Zero backends.

The SPIR-V was extracted from the
[sycl-spirv-hello-world](https://github.com/szymon-zadworny/sycl-spirv-hello-world/tree/offload)
repository.

## Development environment
This repository provides a liboffload development environment Nix flake. You can use it with:

```bash
nix develop
```

## Build instructions
You can compile the program with:

```bash
meson setup build
meson compile -C build
```

You can then run it with:

```bash
./build/offload-hello-world
```

Set `OFFLOAD_TRACE=1` to trace liboffload API calls.
