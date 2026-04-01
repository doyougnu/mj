{
  description = "J Language MLIR Dialect Development Environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, utils }:
    utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        # Use LLVM 17 or 18 (latest stable MLIR features)
        llvm = pkgs.llvmPackages;
      in
      {
        devShells.default = pkgs.mkShell {
          nativeBuildInputs = with pkgs; [
            cmake
            ninja
            ccache
            llvm.clang
            llvm.lld
            llvm.tblgen
          ];

          buildInputs = with pkgs; [
            llvm.llvm
            llvm.mlir
            llvm.libunwind
            libffi
            libxml2
          ];

          shellHook = ''
          export MLIR_INC_DIR="${llvm.mlir}/include"
          export LLVM_INC_DIR="${llvm.llvm}/include"

          # Path to the libraries
          export MLIR_LIB_DIR="${llvm.mlir}/lib"

          export MLIR_INCLUDE_DIRS=${llvm.mlir}/include
          export LLVM_INCLUDE_DIRS=${llvm.llvm}/include
          export LLVM_TABLEGEN_EXE=${llvm.tblgen}
          echo "--- J-MLIR Development Shell ---"
          echo "MLIR Version: $(mlir-tblgen --version | grep version)"
          # Write .clangd so eglot picks up includes without needing a build
          cat > .clangd <<EOF
          CompileFlags:
            Add:
              -I${pkgs.llvmPackages.mlir}/include
              -I${pkgs.llvmPackages.llvm}/include
              -std=c++20

          Index:
            StandardLibrary: Yes

          InlayHints:
            Enabled: Yes

          CompileFlags:
            Add: [...]

          PathMappings:
            - Local: /store/Programming/c++/mj
              Remote: /home/doyougnu/programming/c++/mj
          EOF
          '';
        };
      });
}
