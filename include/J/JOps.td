// type synonym for the ops
class MJ_Op<string mnemonic, list<Trait> traits = []> :
    Op<MJ_Dialect, mnemonic, traits>;

// Verb Glyph Attribute mirrors j::Expr::Prim
def MJ_Plus      : I32EnumAttrCase<"Plus", 0>;
def MJ_Minus     : I32EnumAttrCase<"Minus", 1>;
def MJ_Star      : I32EnumAttrCase<"Star", 2>;
def MJ_Percent   : I32EnumAttrCase<"Percent", 3>;
def MJ_Caret     : I32EnumAttrCase<"Caret", 4>;
def MJ_Lt        : I32EnumAttrCase<"Lt", 5>;
def MJ_Gt        : I32EnumAttrCase<"Gt", 6>;
def MJ_Bar       : I32EnumAttrCase<"Bar", 7>;
def MJ_Hash      : I32EnumAttrCase<"Hash", 8>;
def MJ_Iota      : I32EnumAttrCase<"Iota", 9>;
def MJ_Eq        : I32EnumAttrCase<"Eq", 10>;
def MJ_Slash     : I32EnumAttrCase<"Slash", 11>;
def MJ_Backslash : I32EnumAttrCase<"Backslash", 12>;
def MJ_Tilde     : I32EnumAttrCase<"Tilde", 13>;
def MJ_At        : I32EnumAttrCase<"At", 14>;
def MJ_Amp       : I32EnumAttrCase<"Amp", 15>;

// add the glyphs as an attribute
def MJ_PrimAttr : I32EnumAttr<"Prim", "J Primitive Glyphs",
  [MJ_Plus, MJ_Minus, MJ_Star, MJ_Percent, MJ_Caret, MJ_Lt, MJ_Gt, MJ_Bar,
   MJ_Hash, MJ_Iota, MJ_Eq, MJ_Slash, MJ_Backslash, MJ_Tilde, MJ_At, MJ_Amp ]> {
  let cppNamespace = "::mj";
}

// The Operation for a Verb (e.g., the '+' in '1 + 2')
def ConstantVerbOp : MJ_Op<"constant_verb", [Pure]> {
  let arguments = (ins MJ_PrimAttr:$glyph);
  let results = (outs AnyType:$res); // TODO: make a specialized 'verb' type
}

def DyadAppOp : MJ_Op<"dyad_app", [Pure]> {
  let summary = "J dyadic application of a verb to two nouns";
  let arguments = (ins
    AnyType:$verb,
    AnyType:$lhs,
    AnyType:$rhs
  );
  let results = (outs AnyType:$res); //TODO: add types
  let hasCanonicalizer = 1;
}