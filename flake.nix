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
            gdb
            llvm.mlir
          ];

          buildInputs = with pkgs; [
            llvm.llvm
            llvm.mlir
            llvm.libunwind
            libffi
            libxml2
          ];

          hardeningDisable = [ "fortify" "fortify3" ];

          MLIR_INC_DIR = "${llvm.mlir.dev}/include";

          shellHook = ''
          export LLVM_TABLEGEN_EXE=${llvm.tblgen}
          export MLIR_TABLEGEN_PATH=${llvm.tblgen}/bin/mlir-tblgen
          export CC=clang
          export CXX=clang++
          echo "--- J-MLIR Development Shell ---"
          echo "MLIR Version: $(mlir-tblgen --version | grep version)"
          # Write .clangd so eglot picks up includes without needing a build
          cat > .clangd <<EOF
          CompileFlags:
            Add:
              -I${llvm.mlir.dev}/include
              -I${llvm.llvm.dev}/include
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
