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
            export MLIR_DIR=${llvm.mlir}
            export LLVM_DIR=${llvm.llvm}
            export LLVM_TABLEGEN_EXE=${llvm.tblgen}
            echo "--- J-MLIR Development Shell ---"
            echo "MLIR Version: $(mlir-tblgen --version | grep version)"
          '';
        };
      });
}
