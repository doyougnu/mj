# An out-of-tree MLIR dialect

TODO: rewrite this to explain mj.

This is an example of an out-of-tree [MLIR](https://mlir.llvm.org/) dialect along with a standalone `opt`-like tool to operate on that dialect.

## Building - Component Build

```sh
nix develop
cd build
cmake -G Ninja .. -DMLIR_DIR=$MLIR_DIR -DLLVM_DIR=$LLVM_DIR
ninja
```
