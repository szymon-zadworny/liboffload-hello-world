# liboffload hello world

An upstream LLVM liboffload development environment for Intel GPUs through the Level Zero plugin.

## Development environment
This repository provides an upstream LLVM liboffload development environment Nix flake. You can use
it with:

```bash
nix develop
```

## Build instructions
The build expects the program in `src/main.c`. First you have to set up the build directory.
Execute this from the repository root:

```bash
meson setup build
```

You can then compile the program:

| Target | Command                  | Output                      |
| ------ | ------------------------ | --------------------------- |
| ELF    | `meson compile -C build` | `build/offload-hello-world` |

Set `OFFLOAD_TRACE=1` to trace liboffload API calls.
