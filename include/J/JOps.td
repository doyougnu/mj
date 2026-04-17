#ifndef MJ_OPS
#define MJ_OPS

include "mlir/IR/OpBase.td"
include "mlir/Interfaces/SideEffectInterfaces.td"
include "mlir/IR/EnumAttr.td"

include "J/MJDialect.td"
include "J/MJAttrs.td"

// type synonym for the ops
class MJ_Op<string mnemonic, list<Trait> traits = []> :
    Op<MJDialect, mnemonic, traits>;

def ConstantOp : MJ_Op<"constant", [Pure, ConstantLike]> {
  let summary = "A constant value in the J language.";
  let description = [{
    The constant operation creates a literal value. It takes an attribute
    containing the value (integer, float, or tensor) and returns a single
    result of that type.
  }];

  let arguments = (ins AnyAttr:$value);
  let results = (outs AnyType:$res);

  // ensure that input type == output type for constants
  let builders = [
    OpBuilder<(ins "::mlir::TypedAttr":$value), [{
      build($_builder, $_state, value.getType(), value);
    }]>
  ];
}

// The Operation for a Verb (e.g., the '+' in '1 + 2')
def VerbOp : MJ_Op<"verb", [Pure, ConstantLike]> {
  let arguments = (ins MJ_PrimAttr:$glyph);
  let results = (outs AnyType:$res); // TODO: make a specialized 'verb' type
}

def DyadAppOp : MJ_Op<"dyad_app", [Pure]> {
  let summary = "J dyadic application of a verb to two nouns";
  let arguments = (ins
    AnyType:$verb, // must be anytype to so that verbs are first class, will be a VerbOp
    AnyType:$lhs,
    AnyType:$rhs
  );
  let results = (outs AnyType:$res); //TODO: add types
  // TODO: let hasCanonicalizer = 1;
}

#endif // MJ_OPS