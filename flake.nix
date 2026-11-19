{
  # Upstream LLVM with liboffload, from the llvm-offload flake.
  # Following its nixpkgs keeps the toolchain identical to what it was built with.
  inputs = {
    llvm-offload.url = "github:szymon-zadworny/llvm-offload";
    nixpkgs.follows = "llvm-offload/nixpkgs";
  };

  outputs = { nixpkgs, llvm-offload, ... }:
    let
      pkgs = nixpkgs.legacyPackages.x86_64-linux;
      # The compiler's setup hook also points the Level Zero loader at the Intel GPU driver.
      stdenv = pkgs.overrideCC pkgs.stdenv llvm-offload.packages.x86_64-linux.clang;
    in
    {
      devShells.x86_64-linux.default = pkgs.mkShell.override { inherit stdenv; } {
        packages = with pkgs; [ meson ninja wild git clang-tools spirv-tools ];
        # Provides the liboffload headers and library to the compiler wrapper.
        buildInputs = [ llvm-offload.packages.x86_64-linux.llvm-offload ];
        CC_LD = "wild";
      };
    };
}
