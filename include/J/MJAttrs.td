#ifndef MJ_ATTRS
#define MJ_ATTRS

include "mlir/IR/AttrTypeBase.td"
include "mlir/IR/EnumAttr.td"


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

def MJ_PrimAttr : I32EnumAttr<"Prim", "J Primitive Glyphs",
  [MJ_Plus, MJ_Minus, MJ_Star, MJ_Percent, MJ_Caret, MJ_Lt, MJ_Gt, MJ_Bar,
   MJ_Hash, MJ_Iota, MJ_Eq, MJ_Slash, MJ_Backslash, MJ_Tilde, MJ_At, MJ_Amp]> {
  let cppNamespace = "::mj";
  let genSpecializedAttr = 1;  // generates PrimAttr wrapper class
}

#endif // MJ_ATTRS